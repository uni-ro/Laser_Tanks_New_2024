#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "ESP_NOW_Daniel/comms.h"

uint8_t master_address[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t led_pulse = 0;



void HandleCommand(uint8_t command, uint8_t parameter) {
	
	if (command == 0x00) {
		// Echo
		
		ReturnPingESPNow(master_address, 2);
		
	}
	if (command == 0x02) {
		
		if (parameter < 127) {
			digitalWrite(5, HIGH);
		}
		else {
			digitalWrite(5, LOW);
		}
		
	}
	if (command == 0x03) {
		
		if (parameter < 127) {
			digitalWrite(4, HIGH);
		}
		else {
			digitalWrite(4, LOW);
		}
		
	}
	
}

void ESPNowCallback(uint8_t *mac_addr, uint8_t *data, uint8_t length) {
	
	uint8_t *commandBuffer = UnpackCommandsESPNow(data, length);
	uint8_t *parameterBuffer = UnpackParametersESPNow(data, length);
	
	for (uint8_t i = 0; i < length / 2; i ++) {
		HandleCommand(commandBuffer[i], parameterBuffer[i]);
	}
	
	free(commandBuffer);
	free(parameterBuffer);
	
}
void SerialCallback(uint8_t *messageData) {
	
	uint8_t numCommands = UnpackNumCommandsSerial(messageData);
	uint8_t *commandBuffer = UnpackCommandsSerial(messageData);
	uint8_t *parameterBuffer = UnpackParametersSerial(messageData);

	for (uint8_t i = 0; i < numCommands; i ++) {
		HandleCommand(commandBuffer[i], parameterBuffer[i]);
	}
	
	free(commandBuffer);
	free(parameterBuffer);
	
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
	esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
	esp_now_register_recv_cb(ESPNowCallback);
	
	
}

void setup() {
	
	Serial.begin(9600);
	Serial.println();
	Serial.println();
	Serial.println("This board is the SLAVE");
	Serial.print("MAC Address: ");
	Serial.println(WiFi.macAddress());
	
	InitESPNow();
	
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	
	//digitalWrite(4, HIGH);
	//digitalWrite(5, HIGH);
	
	Comms_Init();
	
}

void loop() {
	
	delay(50);

	if (ReadReadySerial() == true) {
		
		uint8_t *messageData = ReadMessageSerial();
		if (messageData != NULL) {
			SerialCallback(messageData);
			free(messageData);
		}
		
		
	}
	
}