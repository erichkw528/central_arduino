
#ifndef _SPEED_SENSOR_h
#define _SPEED_SENSOR_h
#include <Arduino.h>
#include "base_module.h"
#define SPEED_QUEUE_LENGTH 20
class SpeedSensor : public BaseModule
{
const float WHEEL_CIRCUMFERENCE = 5.75;   // Circumference of the wheel in inches
const float INCHES_PER_MILE = 63360.0;     // Number of inches in a mile
const unsigned long DEBOUNCE_DELAY = 100;  // Debounce delay in milliseconds
static void pulseCounter();

public:
    SpeedSensor(uint32_t speed_sensor_pin);
    Status setup();
    Status loop();
    Status cleanup();
    int getCurrentSpeed();
    void update(float targetSpeed, float throttle_effort);
private:
    uint32_t pin;
    unsigned long prevTime = 0; // Time of the previous pulse
    unsigned long prevDebounceTime = 0;
    float currentSpeedMph = 0;
    uint32_t speedQueueLength = SPEED_QUEUE_LENGTH;
    float speedQueue[SPEED_QUEUE_LENGTH];
    uint32_t currentIndex = 0;
    float sensorReadingMph = 0;
    void addSpeedReading(float speed);
    
};


#endif // _SPEED_SENSOR_h