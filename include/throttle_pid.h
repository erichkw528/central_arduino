#ifndef THROTTLE_PID_h
#define THROTTLE_PID_h
#include "base_module.h"

#include <CircularBuffer.h>
class ThrottlePIDController : public BaseModule
{
    public: 
        ThrottlePIDController(float kp, 
                    float kd, 
                    float ki, 
                    float min_output, 
                    float max_output);
        Status setup();
        Status loop();
        Status cleanup();

        float compute(float value, float target);
        float kp = 1;
        float kd = 0;
        float ki = 0;

    private:


        float min_output = -1;
        float max_output = 1;

        float curr_err = 0;
        float total_err = 0;
        float prev_err = 0;
        float time_last = 0;
        float output = 0;
        unsigned long pM = 0UL;
        unsigned long interval = 100UL;
};

#endif // THROTTLE_PID_h