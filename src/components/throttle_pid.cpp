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
    return Status::OK;
}

Status ThrottlePIDController::loop() {
    return Status::OK;
}

Status ThrottlePIDController::cleanup() {
    return Status::OK;
}

float ThrottlePIDController::compute(float value, float target) 
{
    unsigned long cM = millis();
    if (cM - pM > interval)
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
        if ((error*kp<1)&&(error*kp>-1)) total_err = total_err + (error/500);
        else total_err = 0; 
        total_err = MAX(0, MIN(12, total_err));
       
        if (error == 0){
            total_err=0;
        }
        
        
         //Serial.print(" target:");
        // Serial.print(target);
        // Serial.print(" value:");
        // Serial.print(target);
        // Serial.print(" error:");
        // Serial.print(error);
        // Serial.print(" total_err:");
        // Serial.print(total_err);
        output = error * this->kp + (error - prev_err)/dt * this->kd + this->total_err * this->ki;

        //  Serial.print(" prev_err:");
        //  Serial.print(prev_err);
        //  Serial.print(" dt:");
        //  Serial.print(dt);
        //  Serial.print(" total_err:");
        //  Serial.print(total_err);
        //  Serial.print(" error:");
        //  Serial.print(error); 
        //  Serial.println();      
        // Serial.print(" kp:");
        // Serial.print(kp);
        // Serial.print(" kd:");
        // Serial.print(kd);
         //Serial.print(" ki:");
        // Serial.print(ki);

    }
    prev_err = error;
    time_last = time_now;
       return MAX(this->min_output, MIN(this->max_output, output)) ; //;CLIP(output, this->min_output, this->max_output);
    pM = cM;
    }
}