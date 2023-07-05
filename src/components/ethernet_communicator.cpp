// Copyright 2023 michael. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
#include <Arduino.h>
#include <cstring> // Include the <cstring> header for memcpy
#include <ArduinoJson.h>
#include "ethernet_communicator.h"
#define UDP_PACKET_MAX_SIZE_CUSTOM 128
byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(10, 42, 0, 4);
unsigned int localPort = 1883;                 // local port to listen on
char packetBuffer[UDP_PACKET_MAX_SIZE_CUSTOM]; // buffer to hold incoming packet,
EthernetUDP Udp;

EthernetCommunicator::EthernetCommunicator()
{
}
Status EthernetCommunicator::setup()
{
    Ethernet.begin(mac, ip);
    Udp.begin(localPort);
    return Status::OK;
}
Status EthernetCommunicator::loop()
{
    // if there's data available, read a packet
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        this->readUDP();

        this->writeStateToUDP();
    }

    return Status::OK;
}
Status EthernetCommunicator::cleanup()
{
    return Status::OK;
}

void EthernetCommunicator::setVehicleState(VehicleState state)
{
    this->latest_vehicle_state = state;
}

ActuationModelFromEthernet EthernetCommunicator::getAction()
{
    return this->actuation_received;
}
void EthernetCommunicator::writeStateToUDP()
{
    VehicleState vehicleState;
    vehicleState.angle = 45.0f;
    vehicleState.speed = 100.0f;
    vehicleState.steering = 20.0f;

    DynamicJsonDocument jsonDocument(512);

    JsonObject root = jsonDocument.to<JsonObject>();
    root["is_auto"] = vehicleState.is_auto;
    root["is_left_limiter_ON"] = vehicleState.is_left_limiter_ON;
    root["is_right_limiter_ON"] = vehicleState.is_right_limiter_ON;
    root["angle"] = vehicleState.angle;
    root["angular_velocity"] = vehicleState.angular_velocity;
    root["speed"] = vehicleState.speed;
    root["steering"] = vehicleState.steering;

    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    serializeJson(jsonDocument, Udp);
    Udp.endPacket();
}

ActuationModelFromEthernet EthernetCommunicator::readUDP()
{
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

    Serial.println("Contents:");
    Serial.println(packetBuffer);

    ActuationModelFromEthernet act;
    return act;
}
