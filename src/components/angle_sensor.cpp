#include <Arduino.h>

#include "pin.h"
#include "angle_sensor.h"

float measureAngle()
{
    float sensorValue = analogRead(STEERING_ANGLE_SENSOR);
    float angle = (sensorValue - 0) / (1023 - 0) * (RIGHT_ANGLE - LEFT_ANGLE) + LEFT_ANGLE;
    return angle;
}

AngleSensor::AngleSensor(int pin)
{
    this->pin = pin;
}

Status AngleSensor::setup()
{
    if (this->pin)
    {
        return Status::SUCCESS;
    }
    return Status::FAILED;
}

Status AngleSensor::loop()
{
    if (!this->pin)
    {
        return Status::FAILED;
    }
    
    float sensorValue = analogRead(this->pin);
    float angle = (sensorValue - 0) / (1023 - 0) * (this->RIGHT_ANGLE - this->LEFT_ANGLE) + this->LEFT_ANGLE;
    this->currentAngle = angle;
    return Status::SUCCESS;
}

Status AngleSensor::cleanup()
{
    return Status::SUCCESS;
}