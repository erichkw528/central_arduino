
#ifndef _SPEED_SENSOR_h
#define _SPEED_SENSOR_h
#include <Arduino.h>
#include "base_module.h"
#include <CircularBuffer.h>

#define SPEED_QUEUE_LENGTH 20
class SpeedSensor : public BaseModule
{
const float WHEEL_CIRCUMFERENCE = 5.75;   // Circumference of the wheel in inches
const float INCHES_PER_MILE = 63360.0;     // Number of inches in a mile
const unsigned long DEBOUNCE_DELAY = 100;  // Debounce delay in milliseconds
const float PREVIOUS_READING_WEIGHT = 1.0;
const float NEW_READING_WEIGHT = 1.0;


static void pulseCounter();

public:
    SpeedSensor(uint32_t speed_sensor_pin);
    Status setup();
    Status loop();
    Status cleanup();
    float getAvgSpeed();

private:
    uint32_t pin;
    unsigned long prevTime = 0; // Time of the previous pulse
    unsigned long prevDebounceTime = 0;
    float lastSensorReadingMph = 0;


    CircularBuffer<float, SPEED_QUEUE_LENGTH> raw_reading_buffer;
    void addSpeedReading(float speed);
    float getLatestReading();

    
};


#endif // _SPEED_SENSOR_h