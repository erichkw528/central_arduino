#include <main.h>
#include <assert.h>

void setup()
{
  Serial.begin(115200);
  vehicle_state = new VehicleState();
  setupModules();
  Serial.println("Setup completed");
}

void loop()
{

  synchronizeModules(); 
  module_manager->loop();
  unsigned long currentTime = millis();  // Current time in milliseconds

  actuation_module->actuate(vehicle_state);
  prevControlTime = currentTime;
  
  Serial.println();
}

void setupModules()
{
  module_manager = new ModuleManager();
  
  led_module = new LEDModule(LED_BUILTIN, 500);
  module_manager->setupModule(led_module);

  steering_angle_sensor = new SteeringAngleSensor(STEERING_ANGLE_SENSOR);
  module_manager->setupModule(steering_angle_sensor);

  pwm_to_voltage_converter = new PWMVoltageConverterModule(THROTTLE_OUTPUT_PIN);
  module_manager->setupModule(pwm_to_voltage_converter);

  radio_link = new RadioLinkModule(THROTTLE_SOURCE, STEERING_SOURCE, BRAKE_SOURCE, BUTTON_SOURCE);
  module_manager->setupModule(radio_link);

  speed_sensor = new SpeedSensor(SPEED_SENSOR_INPUT_PIN);
  module_manager->setupModule(speed_sensor);

  steering_limiter = new SteeringLimiter(STEERING_LEFT_LIMITER, STEERING_RIGHT_LIMITER);
  module_manager->setupModule(steering_limiter);

  spark_max_module = new SparkMaxModule(STEERING_OUTPUT_PIN);
  module_manager->setupModule(spark_max_module);

  brake_actuator = new BrakeActuator();
  module_manager->setupModule(brake_actuator);

  actuation_module = new ActuationModule(steering_limiter, pwm_to_voltage_converter, spark_max_module, brake_actuator);
  module_manager->setupModule(actuation_module);

  steering_pid = new PIDController(0.03, 0, 0, -1.0, 1.0);
  module_manager->setupModule(steering_pid);


  throttle_pid = new ThrottlePIDController(0.16, 0.0, 0.07, 0.0, 1.0);
  module_manager->setupModule(throttle_pid);


  // serial_communicator = new SerialCommunicator();
  // module_manager->setupModule(serial_communicator);
}

void synchronizeModules()
{
  // get data from angle sensor, steering limiter and update vehicle state
  vehicle_state->angle = steering_angle_sensor->getSteeringAngle();
  // vehicle_state->angular_velocity = steering_angle_sensor->getAngularVelocity();
  vehicle_state->is_left_limiter_ON = steering_limiter->isLeftLimiterON();
  vehicle_state->is_right_limiter_ON = steering_limiter->isRightLimiterON();
  vehicle_state->speed = speed_sensor->getCurrentSpeed();

  float target_steering_angle_deg = 0;
  float target_speed = 0;
  if (radio_link->isAutoFromButton()) {
    // get data from serial communicator
    // vehicle_state->current_actuation->throttle = serial_communicator->getAction()->throttle;
    // vehicle_state->current_actuation->brake = serial_communicator->getAction()->throttle;
    // vehicle_state->current_actuation->steering = serial_communicator->getAction()->steering;
    target_steering_angle_deg = serial_communicator->getAction()->steering;
  } else {
    // get data from radio link
    // vehicle_state->current_actuation->throttle = radio_link->getThrottle();
    vehicle_state->current_actuation->brake = radio_link->getBrake();
    target_steering_angle_deg = radio_link->getSteeringDeg();
    target_speed = radio_link->getTargetSpeed();
  }
  // run PID
  float steering_effort = steering_pid->compute(vehicle_state->angle, target_steering_angle_deg);
  float throttle_effort = throttle_pid->compute(vehicle_state->speed, target_speed);

  vehicle_state->current_actuation->steering = steering_effort; //radio_link->getSteering(); // actually sending steering
  vehicle_state->current_actuation->throttle = throttle_effort;
  
  speed_sensor->update(target_speed, throttle_effort); // update speed sensor using some rough estimates

  // Serial.print(" radioLink Steering: ");
  // Serial.print(target_steering_angle_deg);
  // Serial.print(" Output Steering: ");
  // Serial.print(steering_effort);
  /**
   * PID tuning
  */
  // Serial.print(" Current: ");
  // Serial.print(vehicle_state->speed);

  // Serial.print(" Target: ");
  // Serial.print(target_speed);

  // Serial.print(" Throttle: ");
  // Serial.print(throttle_effort);

  // int val = analogRead(A5);
  // float kp = (val - 0) / (1023.0) * (1.0);
  // throttle_pid->kp = kp;
  // Serial.print(" kp: ");
  // Serial.print(throttle_pid->kp);


  // Serial.print(" kd: ");
  // Serial.print(throttle_pid->kd);

  // val = analogRead(A4);
  // float kd = (val - 0) / (1023.0) * (1.0);
  // throttle_pid->ki = kd;
  // Serial.print(" ki: ");
  // Serial.print(throttle_pid->ki);
}