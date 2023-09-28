#include "relay_module.h"

RelayController::RelayController()
{
    this->name = "Relay Controller";
}

Status RelayController::setup()
{
    Serial.begin(115200);
    for (int i = 0; i < NUM_RELAYS; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], LOW);
    }
    return Status::OK;
}

Status RelayController::loop()
{
    if (Serial.available()) {
        receivedData = Serial.read();

        if (receivedData == 'A') {
            updateRelayStates();
        } 
        else {  
            pendingState = receivedData;
            Serial.write(receivedData);
        }

    }

        return Status::OK;
}

Status RelayController::cleanup()
{
    return Status::OK;
}


void RelayController::updateRelayStates()
{
    for (int i = 0; i < NUM_RELAYS; i++) {
        digitalWrite(relayPins[i], (pendingState >> i) & 1);
    }
}