#ifndef _STATE_COLLECTION_H
#define _STATE_COLLECTION_H

#include "base_module.h"
#include <Arduino.h> 
#include "models.h"

#define ACK_TIMEOUT 1000  

class StateCollector : public BaseModule
{
public:
    StateCollector();
    Status setup();
    Status cleanup();

    void write_states(Actuation *act, bool isForward);
    
private:
    byte collectStates(bool isForward);

    bool prev_isForward = false;
};

#endif
