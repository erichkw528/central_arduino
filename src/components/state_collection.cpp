#include "state_collection.h"

// StateCollector::StateCollector(LEDModule* ledModule, Module2* module2, ... , Module8* module8)
//     : _ledModule(ledModule), _module2(module2), /* ... */ _module8(module8)
// {
//     this->name = "State Collector";
// }

StateCollector::StateCollector()
{
    this->name = "State Collector";
}

Status StateCollector::setup() 
{
    Serial1.begin(115200); 
    return Status::OK;
}

byte StateCollector::collectStates(bool isReverse) 
{
    byte state = 0;
    if (isReverse) { 
        state |= 1 << 0; // Set the first bit of the state to reverse status
    }
    // If more states need to be added, continue here
    return state;
}

void StateCollector::write_states(bool isReverse) 
{
    if (prev_isReverse != isReverse) {
        byte sentState = collectStates(isReverse); // Collect states with the isReverse parameter

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
        delay(1000);

        prev_isReverse = isReverse; // Update the previous reverse status after sending
    }
}


Status StateCollector::cleanup()
{
    return Status::OK;
}