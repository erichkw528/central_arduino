#include "state_collection.h"

StateCollector::StateCollector()
{
    this->name = "State Collector";
}

Status StateCollector::setup()
{
    Serial1.begin(115200);
    return Status::OK;
}

byte StateCollector::collectStates(bool isForward)
{
    byte bits[8];
    bits[0] = isForward;  // Assuming isForward is either 0 or 1
    bits[1] = !isForward; // isReverse is either 0 or 1
    bits[2] = 0;
    bits[3] = 0;
    bits[4] = 0;
    bits[5] = 0;
    bits[6] = 0;
    bits[7] = 0;

    byte state = 0;

    for (int i = 0; i < 8; i++)
    {
        state |= (bits[i] << i);
    }

    return state;
}

void StateCollector::write_states(Actuation *act, float current_speed, float throttle_effort, bool isForward)
{
    // If reverse is not allowed, exit early
    if (!act->reverse || current_speed >= 1)
    {
        return;
    }

    // Generate the initial state based on the isForward parameter
    byte sentState = collectStates(isForward);

    // If the car is not moving and there's no throttle input, clear the first two bits
    if (throttle_effort == 0 && current_speed < 1)
    {
        sentState &= ~0b00000011;
    }

    // Only send data when speed is less tha 1.
    if (current_speed < 1 && sentState != lastSentState)
    {
        // Transmit the state to the esp8266
        Serial1.write(sentState);
        Serial1.flush();

        unsigned long startTime = millis();
        bool ackReceived = false;
        // Wait for an acknowledgment or timeout
        while (millis() - startTime < ACK_TIMEOUT && !ackReceived)
        {
            if (Serial1.available())
            {
                byte receivedState = Serial1.read();
                if (receivedState == sentState)
                {
                    ackReceived = true;
                    Serial1.write('A'); // Send acknowledgment response
                }
            }
        }

        lastSentState = sentState;
    }
    Serial.print("state");
    Serial.print(isForward);
}

Status StateCollector::cleanup()
{
    return Status::OK;
}