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
    actuation_module->actuate(vehicle_state);
    ethernet_communicator->setVehicleState(*vehicle_state);
}

void setupModules()
{
    module_manager = new ModuleManager();

    led_module = new LEDModule(LED_BUILTIN);
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

    throttle_pid = new ThrottlePIDController(0.14, 0.07, 0.07, 0.0, 1.0);
    module_manager->setupModule(throttle_pid);

    ethernet_communicator = new EthernetCommunicator();
    module_manager->setupModule(ethernet_communicator);
}

void synchronizeModules()
{
    // get data from angle sensor, steering limiter and update vehicle state
    vehicle_state->current_angle = steering_angle_sensor->getSteeringAngle();
    vehicle_state->current_angular_velocity = steering_angle_sensor->getAngularVelocity();
    vehicle_state->is_left_limiter_ON = steering_limiter->isLeftLimiterON();
    vehicle_state->is_right_limiter_ON = steering_limiter->isRightLimiterON();
    vehicle_state->current_speed = speed_sensor->getAvgSpeed();

    float target_steering_angle_deg = 0;
    float target_speed = 0;
    if (radio_link->isAutoFromButton())
    {
        led_module->setMode(LEDMode::AUTONOMOUS_MODE);

        // get data from serial communicator
        vehicle_state->current_actuation->brake = ethernet_communicator->getAction().brake;
        target_speed = ethernet_communicator->getAction().targetSpeed;
        target_steering_angle_deg = ethernet_communicator->getAction().steeringAngle;
    }
    else
    {
        led_module->setMode(LEDMode::MANUAL_MODE);

        // get data from radio link
        vehicle_state->current_actuation->brake = radio_link->getBrake();
        target_steering_angle_deg = radio_link->getSteeringDeg();
        target_speed = radio_link->getTargetSpeed();
    }
    // set state
    vehicle_state->target_speed = target_speed;
    vehicle_state->target_steering_angle = target_steering_angle_deg;
    // run PID
    float steering_effort = steering_pid->compute(vehicle_state->current_angle, target_steering_angle_deg);
    float throttle_effort = throttle_pid->compute(vehicle_state->current_speed, target_speed);

    vehicle_state->current_actuation->steering = steering_effort;
    vehicle_state->current_actuation->throttle = throttle_effort;

    brake_actuator->setSpeedError(abs(vehicle_state->current_speed-vehicle_state->target_speed));
    
}