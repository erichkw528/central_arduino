/**
 * By Michael Wu on July 9, 2022.
 * reference from https://www.camelsoftware.com/2015/12/25/reading-pwm-signals-from-an-rc-receiver-with-arduino/
 *
 * Please wire up your PWM Inputs on Digital Pins and revise the *_SOURCE fields to the corresponding pin numbers.
 * only arduino ble nano 33 and arduino due are tested
 *
 **/
#include <main.h>


void setup()
{
  Serial.begin(9600);
  setupPwmVoltageConverter();
  setupLED();
  setupSparkMax();
  setupRadioLink();  
}

void loop()
{
  // printRawStatus();
  // if button is pressed, use serial inputs
  if (button_pulse_time > 1500)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    changeSteeringToNeutral();
    changeThrottleToNeutral();
    changeBrakeToNeutral();
    // TODO: parse serial inputs
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    // if button is not pressed, serial input is ignored, use controller input
    output_throttle_pwm = throttle_pulse_time;
    output_steering_pwm = steering_pulse_time;
    output_brake_pwm = brake_pulse_time;
  }

  actuateBrake();
  actuateSteering();
  output_throttle_pwm = 2000;
  actuateThrottle();
  
}

void actuateSteering() {
  writeToSteering();
}

void actuateBrake() {
  writeToBrake();
}

void actuateThrottle() {
  writeToThrottle();
}