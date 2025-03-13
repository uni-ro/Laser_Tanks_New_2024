#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "ESP_NOW_Daniel/comms.h"

uint8_t slave_address[] = {0x80, 0x7D, 0x3A, 0x23, 0x7F, 0x58};



void ESPNowCallback(uint8_t *mac_addr, uint8_t *data, uint8_t length) {
	
	Serial.write(0xFF);
	for (int i = 0; i < length; i ++) {
		Serial.write(data[i]);
	}
	Serial.write(0xFE);
	
}



void InitESPNow() {
	
	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	if (esp_now_init() == 0) {
		Serial.println("Success initialising ESPNow");
	}
	else {
		Serial.println("Failed to initialise ESPNow");
		Serial.println("Retrying in 3 seconds");
		delay(3000);
		InitESPNow();
	}
	
	// ESPNow has initialised successfully here
	esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
	esp_now_add_peer(slave_address, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
	esp_now_register_recv_cb(ESPNowCallback);
		
}

void setup() {
	
	Serial.begin(9600);
	Serial.println();
	Serial.println();
	Serial.println("This board is the MASTER");
	Serial.print("MAC Address: ");
	Serial.println(WiFi.macAddress());
	
	InitESPNow();
	
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, !LOW);
	
	Comms_Init();
	
}


uint8_t serialMessage[255];
uint8_t serialLength;
void loop() {
	

	delay(50);
	digitalWrite(LED_BUILTIN, !LOW);
	
	
	while (Serial.available() > 0) {
		uint8_t currentByte = Serial.read();
		
		// Start byte
		if (currentByte == 0xFF) {
			serialLength = 0;
			continue;
		}
		
		// End byte
		if (currentByte == 0xFE) {
			esp_now_send(slave_address, serialMessage, serialLength);
			digitalWrite(LED_BUILTIN, !HIGH);
			delay(5);
			digitalWrite(LED_BUILTIN, !LOW);
			continue;
		}
		
		serialMessage[serialLength] = currentByte;
		serialLength ++;
		
	}
	
}