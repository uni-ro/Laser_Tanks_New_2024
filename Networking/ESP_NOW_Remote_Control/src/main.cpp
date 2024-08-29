//ESP32
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
typedef struct struct_message
{
    char macLocal[MAC_ADDR_SIZE];
    char shooterID[SHOOTER_ID_SIZE];
    char extraMessage[11] = "Hit by you";

} struct_message;

struct_message message;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
}

