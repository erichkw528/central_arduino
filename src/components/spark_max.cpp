#include "spark_max.h"
int output_steering_pwm = 1500;
SparkMaxModule::SparkMaxModule(uint32_t pin)
{
    this->pin = pin;
}

Status SparkMaxModule::setup()
{
    servoSteering.attach(this->pin);
    return Status::OK;
}
Status SparkMaxModule::loop()
{
    this->checkServoAttachments();
    return Status::OK;
}
Status SparkMaxModule::cleanup()
{
    return Status::OK;
}

void SparkMaxModule::checkServoAttachments()
{
    if (servoSteering.attached() == false)
    {
        servoSteering.attach(this->pin);
    }
}

void SparkMaxModule::writeToSteering(float steering)
{
    // convert [-1, 1] to [1000, 2000]
    // Serial.print("Steering: ");
    // Serial.print(steering);

    int steering_pwm = (steering - -1) / (1 - -1) * (OUTPUT_STEERING_MAX - OUTPUT_STEERING_MIN) + OUTPUT_STEERING_MIN;
    // Serial.print(" Steering pwm: ");
    // Serial.print(steering_pwm);
    servoSteering.writeMicroseconds(constrain(steering_pwm, OUTPUT_STEERING_MIN, OUTPUT_STEERING_MAX));
}
