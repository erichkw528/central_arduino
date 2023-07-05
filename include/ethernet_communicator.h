// Copyright 2023 michael. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "models.h"
#include "base_module.h"
#include <Dhcp.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h> // UDP library from: bjoern@cs.stanford.edu 12/30/2008

struct ActuationModelFromEthernet
{
  float targetSpeed = 0.0f;
  float steeringAngle = 0.0f;
  float brake = 0.0f;
  bool reverse = false;
};

class EthernetCommunicator : public BaseModule
{

public:
  EthernetCommunicator();
  Status setup();
  Status loop();
  Status cleanup();

  ActuationModelFromEthernet getAction();
  void setVehicleState(VehicleState vehicle_state);

private:
  void writeStateToUDP();
  ActuationModelFromEthernet readUDP();
  // void writeStateToSerial(VehicleState *state,
  //                         char start_marker,
  //                         char end_marker);
  // void parseActionData(char *buf,
  //                            uint32_t len,
  //                            char start_marker,
  //                            char end_marker);
  // void parseSerialData(VehicleState *vehicle_state,
  //                      char start_marker,
  //                      char end_marker,
  //                      uint32_t buf_len = 20);

  // void processSerialCommunication(VehicleState *vehicle_state);

  VehicleState latest_vehicle_state;
  ActuationModelFromEthernet actuation_received;

  // char receivedChars[32]; // make sure this is the same as numChars
  // boolean newData = false;
};
