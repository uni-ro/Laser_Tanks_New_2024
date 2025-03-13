//ESP8266
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <../include/ESPNOWC.h>
#define MAC_ADDR_SIZE 12
#define SHOOTER_ID_SIZE 1
//#define DEBUG


void setup()
{
    // Init ESP
    Serial.begin(75800);
    espNowInit();
    CheckInfo();
    espNowStart();
    //espNowListener();
}

void loop()
{


}

