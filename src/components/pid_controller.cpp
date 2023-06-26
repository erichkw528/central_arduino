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
    
    total_err += error;
    float output = error * this->kp + (error - prev_err) * this->kd + this->total_err * this->ki;
    prev_err = error;

    return  CLIP(output, this->min_output, this->max_output);
}