#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "ESP_NOW_Daniel/comms.h"

uint8_t master_address[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t led_pulse = 0;
bool motorsOn = false;

// Pin D2
#define LEFT_MOTOR_PWM 14
#define LEFT_MOTOR_DIR_A 4
#define LEFT_MOTOR_DIR_B 5

#define RIGHT_MOTOR_PWM 15
#define RIGHT_MOTOR_DIR_A 13
#define RIGHT_MOTOR_DIR_B 12

#define LIGHT_SENSOR A0

void HandleCommand(uint8_t command, uint8_t parameter) {
	
	if (command == 0x00) {
		// Echo
		ReturnPingESPNow(master_address, 2);
		
	}
	if (command == 0x02) {
		// Left motor speed
		analogWrite(LEFT_MOTOR_PWM, parameter);
	}
	if (command == 0x03) {
		// Left motor direction
		digitalWrite(LEFT_MOTOR_DIR_A, parameter ^ 1);
		digitalWrite(LEFT_MOTOR_DIR_B, parameter);

	}
	
	if (command == 0x04) {
		// Right motor speed
		analogWrite(RIGHT_MOTOR_PWM, parameter);
	}
	if (command == 0x05) {
		// Right motor direction
		digitalWrite(RIGHT_MOTOR_DIR_A, parameter ^ 1);
		digitalWrite(RIGHT_MOTOR_DIR_B, parameter);

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
	
	digitalWrite(LED_BUILTIN, !HIGH);
	
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
	pinMode(LED_BUILTIN_AUX, OUTPUT);
	
	pinMode(LEFT_MOTOR_PWM, OUTPUT);
	pinMode(LEFT_MOTOR_DIR_A, OUTPUT);
	pinMode(LEFT_MOTOR_DIR_B, OUTPUT);
	
	pinMode(RIGHT_MOTOR_PWM, OUTPUT);
	pinMode(RIGHT_MOTOR_DIR_A, OUTPUT);
	pinMode(RIGHT_MOTOR_DIR_B, OUTPUT);
	
	//pinMode(LIGHT_SENSOR, INPUT);
	
	Comms_Init();
	
}

int pulseCounter = 0;
void loop() {
	
	delay(50);
	digitalWrite(LED_BUILTIN, ~LOW);
	
	if (pulseCounter == 0 || pulseCounter == 4) {
		digitalWrite(LED_BUILTIN_AUX, !HIGH);
		delay(2);
		digitalWrite(LED_BUILTIN_AUX, !LOW);
	}
	if (pulseCounter == 0) {
		
		int batteryLevel = (analogRead(LIGHT_SENSOR) >> 2);
		if (batteryLevel >= 255) {
			batteryLevel = 255;
		}
		
		//int test = analogRead(LIGHT_SENSOR) >> 3;
		uint8_t command = 0x02;
		uint8_t parameter = batteryLevel;
		SendMessageESPNow(master_address, &command, &parameter, 1);
		
	}
	
	pulseCounter ++;
	pulseCounter %= 16;
	
	
	
	
	/*
	Serial.print(">Battery_Voltage:");
	Serial.print(batteryVoltage);
	
	Serial.print(",Target_Voltage:");
	Serial.print(8.4);
	
	Serial.print(",Minimum_Voltage:");
	Serial.println(6.4);
	*/
	if (ReadReadySerial() == true) {
		
		uint8_t *messageData = ReadMessageSerial();
		if (messageData != NULL) {
			SerialCallback(messageData);
			free(messageData);
		}
		
		
	}
	
}