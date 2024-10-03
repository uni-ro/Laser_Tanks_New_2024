//ESP32
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <../include/esp_now_util.h>

void setup() {
    Serial.begin(75800);
    Serial.print("ESP Board MAC Address:  ");
  // put your setup code here, to run once:
    espNowInitController();
    espNowStart();
    movementInfo.XL=0;
    movementInfo.XR=0;
    movementInfo.YL=0;
    movementInfo.YR=0;
    movementInfo.SelL=0;
    movementInfo.SelR=0;
    updateMovementData();
    SendMovementData();

}

void loop() {
  // put your main code here, to run repeatedly:
}

