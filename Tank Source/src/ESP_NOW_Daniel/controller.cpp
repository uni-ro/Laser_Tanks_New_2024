#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "ESP_NOW_Daniel/comms.h"

uint8_t slave_address[] = {0x80, 0x7D, 0x3A, 0x23, 0x7F, 0x58};



void HandleCommand(uint8_t command, uint8_t parameter) {
	// Senders
	// 0 - PC
	// 1 - Master ESP
	// 2 - Slave ESP
	
	if (command == 0x00) {
		// Ping command
		
		if (parameter == 0) {
			ReturnPingSerial(1);
		}
		else {
			ReturnPingESPNow(slave_address, 1);
		}
		
	}
	
}
void ESPNowCallback(uint8_t *mac_addr, uint8_t *data, uint8_t length) {
	
	uint8_t *commandBuffer = UnpackCommandsESPNow(data, length);
	uint8_t *parameterBuffer = UnpackParametersESPNow(data, length);
	
	for (uint8_t i = 0; i < length / 2; i ++) {
		HandleCommand(commandBuffer[i], parameterBuffer[i]);
	}
	// Relay the message
	SendMessageSerial(commandBuffer, parameterBuffer, length / 2);
	
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
	SendMessageESPNow(slave_address, commandBuffer, parameterBuffer, numCommands);
	
	free(commandBuffer);
	free(parameterBuffer);
	
	digitalWrite(LED_BUILTIN, !HIGH);
	delay(2);
	digitalWrite(LED_BUILTIN, !LOW);
	
	
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



void loop() {
	

	delay(50);
	digitalWrite(LED_BUILTIN, !LOW);
	
	if (ReadReadySerial() == true) {
		
		uint8_t *messageData = ReadMessageSerial();
		if (messageData != NULL) {
			SerialCallback(messageData);
			free(messageData);
		}
		
		
	}
	
	
}