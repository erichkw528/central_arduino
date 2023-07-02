#include "pid_controller.h"
#include <Arduino.h>
#include "utilities.h"

PIDController::PIDController(float kp, float kd, float ki, float min_output, float max_output)
    : kp(kp), kd(kd), ki(ki) {
        this->kp = kp;
        this->kd = kd;
        this->ki = ki;
        this->min_output = min_output;
        this->max_output = max_output;
}

Status PIDController::setup() {
    return Status::SUCCESS;
}

Status PIDController::loop() {
    return Status::SUCCESS;
}

Status PIDController::cleanup() {
    return Status::SUCCESS;
}

float PIDController::compute(float value, float target) 
{
    float error = target - value;
    float time_now = micros();
    float dt = (time_now - time_last)/1e6;
    if (dt == 0) 
    {
       output = 0; 
    } 
    else
    {
        total_err += error; 
        total_err = MAX(0, MIN(2, total_err));
        output = error * this->kp + (error - prev_err)/dt * this->kd + this->total_err * this->ki;
    }
    prev_err = error;
    time_last = time_now;

    return MAX(0, MIN(1, output)) ; //;CLIP(output, this->min_output, this->max_output);
}