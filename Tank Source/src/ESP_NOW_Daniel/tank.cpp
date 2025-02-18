#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "ESP_NOW_Daniel/comms.h"

uint8_t master_address[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
int pulseLED = 0; // When true, the builtin LED will be pulsed during the next loop

#define MOTOR_A_POS 12
#define MOTOR_A_NEG 0

#define MOTOR_B_POS 4
#define MOTOR_B_NEG 14

void SendMessage(uint8_t* message, uint8_t length) {
	
	Serial.write(0xFF);
	for (int i = 0; i < length; i ++) {
		Serial.write(message[i]);
	}
	Serial.write(0xFE);
	
}
void SetLeftMotorSpeed(uint8_t speed, uint8_t direction) {
	
	if (speed == 0) {
		digitalWrite(MOTOR_A_POS, 0);
		digitalWrite(MOTOR_A_NEG, 0);
	}
	
	if (direction == 1) {
		digitalWrite(MOTOR_A_NEG, 0);
		analogWrite(MOTOR_A_POS, speed);
		return;
	}
	if (direction == 0) {
		digitalWrite(MOTOR_A_POS, 0);
		analogWrite(MOTOR_A_NEG, speed);
		return;
	}
	
	
	
}
void SetRightMotorSpeed(uint8_t speed, uint8_t direction) {
	
	if (speed == 0) {
		digitalWrite(MOTOR_B_POS, 0);
		digitalWrite(MOTOR_B_NEG, 0);
		return;
	}
	
	if (direction == 1) {
		digitalWrite(MOTOR_B_NEG, 0);
		analogWrite(MOTOR_B_POS, speed);
		return;
	}
	if (direction == 0) {
		digitalWrite(MOTOR_B_POS, 0);
		analogWrite(MOTOR_B_NEG, speed);
		return;
	}
	
	
	
	
}

void HandleMessage(uint8_t* message, uint8_t length) {
	
	pulseLED += 1;
	
	if (length == 0) { return; }
	
	
	
	// Ping command
	if (message[0] == 0x00) {
		if (length != 2) { return; }
		
		if (message[1] == 0x00) {
			Serial.println("Ping recieved");
			uint8_t message[] = {0x00, 0x01};
			SendMessage(message, 2);
		}
		
	}

	// Left motor speed
	if (message[0] == 0x01) {
		if (length != 2) { return; }
		
		uint8_t motorSpeed = message[1] % 0x40;
		motorSpeed = motorSpeed << 2;
		uint8_t direction = (message[1] >> 6) & 0b1;
		
		Serial.print("Motor_L: ");
		if (direction == 1) { Serial.print("-"); }
		else { Serial.print("+"); }
		Serial.println(motorSpeed);
		
		SetLeftMotorSpeed(motorSpeed, direction);

	}
	
	// Right motor speed
	if (message[0] == 0x02) {
		if (length != 2) { return; }
		
		uint8_t motorSpeed = message[1] % 0x40;
		motorSpeed = motorSpeed << 2;
		uint8_t direction = (message[1] >> 6) & 0b1;
		
		Serial.print("Motor_R: ");
		if (direction == 1) { Serial.print("-"); }
		else { Serial.print("+"); }
		Serial.println(motorSpeed);
		
		SetRightMotorSpeed(motorSpeed, direction);

	}
	
	
	
}

uint8_t serialMessage[255];
uint8_t serialMessageLength = 0;
// Reads the serial messages. If a complete message has been recieved it is passed on to HandleMessage.
void ProcessSerialMessages() {
	
	while (Serial.available() > 0) {
		uint8_t currentByte = Serial.read();
		
		// Start byte
		if (currentByte == 0xFF) {
			serialMessageLength = 0;
			continue;
		}
		
		// End byte
		if (currentByte == 0xFE) {
			HandleMessage(serialMessage, serialMessageLength);
			continue;
		}
		
		serialMessage[serialMessageLength] = currentByte;
		serialMessageLength ++;
		
		
	}

}

#define ESPNOW_MESSAGE_QUEUE_LENGTH 8
uint8_t* espnowMessage[ESPNOW_MESSAGE_QUEUE_LENGTH];
uint8_t espnowMessageLength[ESPNOW_MESSAGE_QUEUE_LENGTH];
void ESPNowCallback(uint8_t *mac_addr, uint8_t *data, uint8_t length) {
	
	uint8_t* espnowData = (uint8_t*)malloc(sizeof(uint8_t) * length);
	memcpy(espnowData, data, length);
	
	for (int i = 0; i < ESPNOW_MESSAGE_QUEUE_LENGTH; i ++) {
		if (espnowMessageLength[i] == 0) {
			espnowMessage[i] = espnowData;
			espnowMessageLength[i] = length;
			break;
		}
	}
	
}
void InitESPNow() {
	
	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	if (esp_now_init() == 0) {
		//Serial.println("Success initialising ESPNow");
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
	Serial.println("This is the Tank board");
	Serial.print("MAC Address: ");
	Serial.println(WiFi.macAddress());
	
	InitESPNow();

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(LED_BUILTIN_AUX, OUTPUT);
	
	pinMode(MOTOR_A_POS, OUTPUT);
	pinMode(MOTOR_A_NEG, OUTPUT);
	
	pinMode(MOTOR_B_POS, OUTPUT);
	pinMode(MOTOR_B_NEG, OUTPUT);
	
	
	Comms_Init();
	
}

uint8_t pulseCounter;
void loop() {
	
	// Pulse the external LED
	while (pulseLED > 0) {
		pulseLED -= 1;
		digitalWrite(LED_BUILTIN, HIGH);
		delay(5);
		digitalWrite(LED_BUILTIN, LOW);
		if (pulseLED > 0) {
			delay(5);
		}
	}
	
	// LED Heartbeat
	if (pulseCounter == 0 || pulseCounter == 40) {
		digitalWrite(LED_BUILTIN_AUX, LOW);
		delay(5);
		digitalWrite(LED_BUILTIN_AUX, HIGH);
		delay(5);
	}
	pulseCounter ++;
	pulseCounter %= 160;
	
	// Read and recieve serial messages
	ProcessSerialMessages();
	
	// Check if ESPNow messages are ready to be handled
	for (int i = 0; i < ESPNOW_MESSAGE_QUEUE_LENGTH; i ++) {
		if (espnowMessageLength[i] > 0) {
			HandleMessage(espnowMessage[i], espnowMessageLength[i]);
			free(espnowMessage[i]);
			espnowMessageLength[i] = 0;
		}
	}
	
	delay(5);
	
}