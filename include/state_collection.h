#ifndef _STATE_COLLECTION_H
#define _STATE_COLLECTION_H

#include "base_module.h"
#include <Arduino.h>
#include "led.h"  
// #include "otherModule.h"

#define ACK_TIMEOUT 1000  

class StateCollector : public BaseModule
{
public:
    //LEDModule* ledModule, Module2* module2, ... , Module8* module8
    StateCollector();
    Status setup();
    Status cleanup();

    void write_states(bool isReverse);
    
private:
    byte collectStates(bool isReverse);
    // LEDModule* _ledModule;
    // Module2* _module2;
    // // ...
    // Module8* _module8;

    bool prev_isReverse = false;
};

#endif
