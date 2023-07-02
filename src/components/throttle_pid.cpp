#include "throttle_pid.h"
#include <Arduino.h>
#include "utilities.h"

ThrottlePIDController::ThrottlePIDController(float kp, float kd, float ki, float min_output, float max_output)
    : kp(kp), kd(kd), ki(ki) {
        this->kp = kp;
        this->kd = kd;
        this->ki = ki;
        this->min_output = min_output;
        this->max_output = max_output;
}

Status ThrottlePIDController::setup() {
    return Status::SUCCESS;
}

Status ThrottlePIDController::loop() {
    return Status::SUCCESS;
}

Status ThrottlePIDController::cleanup() {
    return Status::SUCCESS;
}

float ThrottlePIDController::compute(float value, float target) 
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
        // Serial.print(" target:");
        // Serial.print(target);
        // Serial.print(" value:");
        // Serial.print(target);
        // Serial.print(" error:");
        // Serial.print(error);
        // Serial.print(" total_err:");
        // Serial.print(total_err);
        output = error * this->kp + (error - prev_err)/dt * this->kd + this->total_err * this->ki;
    }
    prev_err = error;
    time_last = time_now;
    return MAX(this->min_output, MIN(this->max_output, output)) ; //;CLIP(output, this->min_output, this->max_output);
}