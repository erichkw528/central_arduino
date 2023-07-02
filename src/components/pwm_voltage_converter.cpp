// Copyright 2023 michael. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <Arduino.h>

#include "pin.h"
#include "pwm_voltage_converter.h"


PWMVoltageConverterModule::PWMVoltageConverterModule(int pin)
{
    this->pin = pin;
}
Status PWMVoltageConverterModule::setup()
{
    pinMode(this->pin, OUTPUT);
    digitalWrite(this->pin, HIGH);
    delay(1000);

    return Status::SUCCESS;
}
Status PWMVoltageConverterModule::loop()
{
    return Status::SUCCESS;

}
Status PWMVoltageConverterModule::cleanup()
{
    this->writeToThrottle(0.0);
    return Status::SUCCESS;
}

void PWMVoltageConverterModule::actuate(float throttle)
{
    throttle = constrain(throttle, 0, 1);
    int output = (throttle - 0) / (1.0 - 0.0) * (225.0 - 0.0) + 0.0;
    analogWrite(this->pin, output);
}

/**
 * @brief  write to throttle
 * @note Instead of executing the throttle directly, it will execute a smoothing function that gradually goes toward the desired throttle
 * @retval None
 */
void PWMVoltageConverterModule::writeToThrottle(float throttle)
{
    smoothWriteThrottle(throttle);
}

void PWMVoltageConverterModule::smoothWriteThrottle(float throttle)
{
    float prevAvg = getPrevAvg();
    float smoothedThrottle = (prevAvg * PREV_THROTTLE_WEIGHT + throttle * CURR_THROTTLE_WEIGHT) / (PREV_THROTTLE_WEIGHT + CURR_THROTTLE_WEIGHT);
    actuate(smoothedThrottle);
}

float PWMVoltageConverterModule::getPrevAvg()
{
    float total = 0;
    for (size_t i = 0; i < buffer.size(); i++)
    {
        total+=buffer[i];
    }
    float avg = total / buffer.size();
    return avg;
}


