// Copyright 2023 michael. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.


#define STEERING_SOURCE 22
#define THROTTLE_SOURCE 23
#define BRAKE_SOURCE 24
#define BUTTON_SOURCE 25
#define STEERING_LEFT_LIMITER 8 // the limitor on the right side of the kart that detect oversteering to the left
#define STEERING_RIGHT_LIMITER 9 // the limitor on the left side of the kart that detect oversteering to the right
#define THROTTLE_OUTPUT_PIN A3 // hardware PWM pin
#define STEERING_OUTPUT_PIN 7 

#define SPEED_SENSOR_INPUT_PIN A0
#define STEERING_ANGLE_SENSOR A1
#define SPI_CS_PIN 9
#define CAN_INT_PIN 2