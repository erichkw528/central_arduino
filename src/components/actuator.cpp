#include "actuator.h"
#include "utilities.h"
ActuationModule::ActuationModule(SteeringLimiter *limiter,
                                 PWMVoltageConverterModule *pwm_to_voltage_converter,
                                 SparkMaxModule *spark_max_module,
                                 BrakeActuator *brake_module
                                 )
{
    this->steering_limiter = limiter;
    this->pwm_to_voltage_converter = pwm_to_voltage_converter;
    this->spark_max_module = spark_max_module;
    this->brake_module = brake_module;
}

Status ActuationModule::setup()
{
    return Status::SUCCESS;
}

Status ActuationModule::loop()
{
    return Status::SUCCESS;
}

Status ActuationModule::cleanup()
{
    return Status::SUCCESS;
}

Actuation * ActuationModule::p_ensure_safety(Actuation *act)
{
    Actuation *output = new Actuation();
    output->brake = act->brake;
    output->reverse = act->reverse;
    output->throttle = MAX(0, act->throttle);
    output->steering = act->steering;

    if (this->steering_limiter->isLeftLimiterON())
    {
        output->steering = MIN(0, act->steering);
    }

    if (this->steering_limiter->isRightLimiterON())
    {
        output->steering = MAX(0, act->steering);
    }
    return output;
}
void ActuationModule::p_drive(VehicleState *vehicle_state)
{
    Actuation *act = this->p_ensure_safety(vehicle_state->current_actuation);
    spark_max_module->writeToSteering(act->steering);
    pwm_to_voltage_converter->writeToThrottle(act->throttle);
    brake_module->writeToBrake(act->brake);
    free(act); // MUST free allocated memory
}

void ActuationModule::actuate(VehicleState *vehicle_state)
{
    p_drive(vehicle_state);
}

