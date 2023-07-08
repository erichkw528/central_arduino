#ifndef MACROS_h
#define MACROS_h

// steering
extern const float MIN_STEERING_DEGREE;
extern const float MAX_STEERING_DEGREE;

// radio link
extern const float RADIO_LINK_MAX_SPEED_GAIN;

// speed sensing // TODO: tune this
#define SPEED_QUEUE_LENGTH 20
extern const float PREVIOUS_READING_WEIGHT; 
extern const float NEW_READING_WEIGHT; 
extern const float WHEEL_CIRCUMFERENCE;   // Circumference of the wheel in inches
extern const float INCHES_PER_MILE;     // Number of inches in a mile
extern const unsigned long DEBOUNCE_DELAY;  // Debounce delay in milliseconds


// throttle // TODO: tune this
#define THROTTLE_REQUEST_BUFFER_LENGTH 20 
extern const float PREV_THROTTLE_WEIGHT; 
extern const float CURR_THROTTLE_WEIGHT; 

// brake
#define CAN_EXT_ID 1
#define CAN_RTR_BIT 8
extern const float MIN_BRAKE_DIST;
extern const float MAX_BRAKE_DIST;
extern const float MIN_EFFECTIVE_SPEED_FOR_BRAKE;

#endif // MACROS_h