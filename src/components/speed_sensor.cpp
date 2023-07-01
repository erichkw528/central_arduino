#include "speed_sensor.h"
static volatile unsigned long pulseCount = 0;   // Number of pulses from the Hall effect sensor

SpeedSensor::SpeedSensor(uint32_t speed_sensor_pin)
{
    this->pin = speed_sensor_pin;
    
}
Status SpeedSensor::setup()
{
    attachInterrupt(this->pin, pulseCounter, CHANGE);

    return Status::SUCCESS;
}

Status SpeedSensor::loop()
{
    unsigned long currentTime = millis();  // Current time in milliseconds
    unsigned long elapsedDebounceTime = currentTime - prevDebounceTime;

    // if not reached debounce time
    if (elapsedDebounceTime < DEBOUNCE_DELAY)
    {
        return Status::SUCCESS;
    }
    prevDebounceTime = currentTime;

    // if reached debounce time
    // calculate how many times did the pulse came in within the elapsed time
    unsigned long elapsedTime = currentTime - prevTime;
    if (elapsedTime < 1000) {
        float distanceInches = pulseCount * (WHEEL_CIRCUMFERENCE / 2.0);  // Calculate distance in inches
        float distanceMiles = distanceInches / INCHES_PER_MILE;           // Convert distance to miles
        float speedMPH = distanceMiles / (elapsedTime / 3600000.0);     // Calculate speed in mph
        currentSpeedMph = speedMPH;
        addSpeedReading(currentSpeedMph);
        // Serial.println(getCurrentSpeed());
    }
    // Reset pulse count and previous time
    pulseCount = 0;
    prevTime = currentTime;

    return Status::SUCCESS;
}

Status SpeedSensor::cleanup()
{
    return Status::SUCCESS;
}


void SpeedSensor::pulseCounter()
{
    pulseCount++;
}

float SpeedSensor::getCurrentSpeed() 
{
    float total = 0;
    for (size_t i = 0; i < speedQueueLength; i++)
    {
        total+= speedQueue[i];
    }
    float avg = total / speedQueueLength;
    
    return avg;
}

void SpeedSensor::addSpeedReading(float speed)
{
    speedQueue[currentIndex % speedQueueLength] = speed;
    currentIndex = (currentIndex + 1) % speedQueueLength;
}
