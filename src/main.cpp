#include <main.h>
#include <assert.h>

void setup()
{
  Serial.begin(115200);
  vehicle_state = new VehicleState();
  setupModules();
}

void loop()
{
  synchronizeModules(); 
  module_manager->loop();
  actuation_module->actuate(vehicle_state);

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

  steering_limiter = new SteeringLimiter(STEERING_LEFT_LIMITER, STEERING_RIGHT_LIMITER);
  module_manager->setupModule(steering_limiter);

  spark_max_module = new SparkMaxModule(STEERING_OUTPUT_PIN);
  module_manager->setupModule(spark_max_module);

  actuation_module = new ActuationModule(steering_limiter, pwm_to_voltage_converter, spark_max_module);
  module_manager->setupModule(actuation_module);

  steering_pid = new PIDController(0.03, 0, 0, -1.0, 1.0);
  module_manager->setupModule(steering_pid);

  brake_actuator = new BrakeActuator();
  module_manager->setupModule(brake_actuator);
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

  float target_steering_angle_deg = 0;
  if (radio_link->isAutoFromButton()) {
    // get data from serial communicator
    // vehicle_state->current_actuation->throttle = serial_communicator->getAction()->throttle;
    // vehicle_state->current_actuation->brake = serial_communicator->getAction()->throttle;
    // vehicle_state->current_actuation->steering = serial_communicator->getAction()->steering;
    target_steering_angle_deg = serial_communicator->getAction()->steering;
  } else {
    // get data from radio link
    vehicle_state->current_actuation->throttle = radio_link->getThrottle();
    vehicle_state->current_actuation->brake = radio_link->getBrake();
    // vehicle_state->current_actuation->steering = radio_link->getSteering();
    target_steering_angle_deg = radio_link->getSteeringDeg();
  }

  // run PID
  float steering_effort = steering_pid->compute(vehicle_state->angle, target_steering_angle_deg);

  vehicle_state->current_actuation->steering = steering_effort; //radio_link->getSteering(); // actually sending steering
}