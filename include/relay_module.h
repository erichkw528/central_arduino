#ifndef _RELAY_MODULE_H
#define _RELAY_MODULE_H

#include "base_module.h"
#include <Arduino.h>

#define NUM_RELAYS 8

class RelayController : public BaseModule
{
public:
    RelayController();
    Status setup();
    Status loop();
    Status cleanup();

private:
    void updateRelayStates();  

    int relayPins[NUM_RELAYS] = {16, 14, 12, 13, 15, 0, 4, 5};
    byte relayState = 0;
    byte pendingState = 0;
    byte receivedData = 0;
};

#endif
