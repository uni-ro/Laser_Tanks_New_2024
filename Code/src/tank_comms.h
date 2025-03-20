#ifndef TANK_COMMS_H
#define TANK_COMMS_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

void TankCommsInit(uint8_t* tank_address);
void SetMotorSpeed(int8_t left_speed, int8_t right_speed);

#endif