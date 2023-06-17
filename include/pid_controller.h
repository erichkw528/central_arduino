#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H
#include "base_module.h"

#include <CircularBuffer.h>
class PIDController : public BaseModule
{
    public: 
        PIDController(float kp, 
                    float kd, 
                    float ki, 
                    float min_output, 
                    float max_output);
        Status setup();
        Status loop();
        Status cleanup();

        float compute(float value, float target);

    private:
        float kp = 1;
        float kd = 0;
        float ki = 0;

        float min_output = -1;
        float max_output = 1;

        float curr_err = 0;
        float total_err = 0;
        float prev_err = 0;
};

#endif //PID_CONTROLLER_h