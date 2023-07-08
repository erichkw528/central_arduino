#include "throttle_pid.h"
#include <Arduino.h>
#include "utilities.h"

ThrottlePIDController::ThrottlePIDController(float kp, float kd, float ki, float min_output, float max_output)
    : kp(kp), kd(kd), ki(ki)
{
    this->kp = kp;
    this->kd = kd;
    this->ki = ki;
    this->min_output = min_output;
    this->max_output = max_output;
    this->name = "ThrottlePIDController";
}

Status ThrottlePIDController::setup()
{
    return Status::OK;
}

Status ThrottlePIDController::loop()
{
    return Status::OK;
}

Status ThrottlePIDController::cleanup()
{
    return Status::OK;
}

float ThrottlePIDController::compute(float value, float target)
{
    unsigned long cM = millis();
    if (cM - pM > interval)
    {

        float error = target - value;
        float time_now = micros();
        float dt = (time_now - time_last) / 1e6;
        if (dt == 0)
        {
            output = 0;
        }
        else
        {    
            if ((error*kp<1)&&(error*kp>-1)) total_err = total_err + (error/500);
            else total_err = 0; 
            total_err = MAX(0, MIN(12, total_err));
        
            if (error == 0){
                total_err=0;
            }
            output = error * this->kp + (error - prev_err)/dt * this->kd + this->total_err * this->ki;
        }
        prev_err = error;
        time_last = time_now;
        pM = cM;
        this->prev_output = MAX(this->min_output, MIN(this->max_output, output)) ;
        return  this->prev_output;
    }
    return this->prev_output;
}
