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
    bits[0] = isForward; // Assuming isForward is either 0 or 1
    bits[1] = !isForward; // isReverse is either 0 or 1
    bits[2] = 0;
    bits[3] = 0;
    bits[4] = 0;
    bits[5] = 0;
    bits[6] = 0;
    bits[7] = 0;

    byte state = 0;

    for(int i = 0; i < 8; i++) {
        state |= (bits[i] << i);
    }

    return state;
}



void StateCollector::write_states(Actuation *act, bool isForward) 
{
    // Only proceed if reverse is allowed
    if (!act->reverse) {
        return;  // Exit the function early if reverse is not allowed
    }

    if (prev_isForward != isForward) {
        byte sentState = collectStates(isForward); // Collect states with the isForward parameter

        // Send to esp8266
        Serial1.write(sentState);
        Serial1.flush();

        unsigned long startTime = millis();
        bool ackReceived = false;
        while (millis() - startTime < ACK_TIMEOUT && !ackReceived) {
            if (Serial1.available()) {
                byte receivedState = Serial1.read();
                if (receivedState == sentState) {
                    ackReceived = true;
                    Serial1.write('A');
                }
            }
        }

        prev_isForward = isForward; // Update the previous forward status after sending
    }
    // Serial.print("state");
    // Serial.print(isForward);
}


Status StateCollector::cleanup()
{
    return Status::OK;
}