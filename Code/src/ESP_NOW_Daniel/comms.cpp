#include "ESP_NOW_Daniel/comms.h"
#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t startByteProg;
int waitForMessage;

void Comms_Init() {
	
	startByteProg = 0;
	waitForMessage = -1;
	
}

// --------Serial methods--------
// Pack messages
uint8_t MessageLengthSerial(uint8_t numData) {
	// 2 bytes for every command / parameter pair
	// 3 bytes at the stary
	// 1 checksum byte
	return numData * 2 + 4;
}
uint8_t *PackMessageSerial(uint8_t *commandBuffer, uint8_t *parameterBuffer, uint8_t numData) {
	
	uint8_t checksum = 0x00;
	uint8_t numBytes = MessageLengthSerial(numData);
	uint8_t *messageData = (uint8_t*)malloc(sizeof(uint8_t) * numBytes);
	
	messageData[0] = 0xFF; checksum += 0xFF;
	messageData[1] = 0x7F; checksum += 0x7F;
	messageData[2] = numData; checksum += numData;
	
	for (uint8_t i = 0; i < numData; i ++) {
		
		uint8_t infoHeader = commandBuffer[i];
		
		messageData[3 + i * 2] = infoHeader; checksum += infoHeader;
		messageData[4 + i * 2] = parameterBuffer[i]; checksum += parameterBuffer[i];
	}
	
	messageData[numBytes - 1] = checksum;
	
	return messageData;
}

// Send messages
void SendMessageSerial(uint8_t *messageData, uint8_t len) {
	for (uint8_t i = 0; i < len; i ++) {
		Serial.write(messageData[i]);
	}
}
void SendMessageSerial(uint8_t *commandBuffer, uint8_t *parameterBuffer, uint8_t numData) {
	
	uint8_t *messageData = PackMessageSerial(commandBuffer, parameterBuffer, numData);
	uint8_t messageLength = MessageLengthSerial(numData);
	SendMessageSerial(messageData, messageLength);
	
	free(messageData);
	
}

// Read messages
bool ReadReadySerial() {
	
	while (waitForMessage == -1 && Serial.available() != 0) {

		uint8_t currentByte = Serial.read();
		
		if (startByteProg == 2) {
			// this byte contains how many commands are on this message
			// total remaining bytes is
			// 2 bytes for each command
			// 1 byte for the checksum
			waitForMessage = currentByte * 2 + 1;
			startByteProg = 0;
		}
		if (startByteProg == 1) {
			if (currentByte == 0x7F) {
				startByteProg ++;
			}
			else {
				startByteProg = 0;
			}
		}
		if (startByteProg == 0) {
			if (currentByte == 0xFF) {
				startByteProg ++;
			}
			else {
				startByteProg = 0;
			}
		}
		
		
	}
	if (waitForMessage != -1 && Serial.available() >= waitForMessage) {
		return true;
	}
	
	return false;
	
}
uint8_t *ReadMessageSerial() {
	
	// waitForMessage contains the number of bytes remaining in this message
	// to get the number of bytes, add 3 for the extra start ones
	uint8_t numBytes = waitForMessage + 3;
	uint8_t numData = (waitForMessage - 1) >> 1;
	uint8_t *messageData = (uint8_t*)malloc(numBytes);
	memset(messageData, 0, numBytes);
	
	// Reconstruct the start bytes for checksum
	messageData[0] = 0xFF;
	messageData[1] = 0x7F;
	messageData[2] = numData;
	
	for (uint8_t i = 0; i < waitForMessage; i ++) {
		messageData[i + 3] = Serial.read();
	}
	
	uint8_t expectedChecksum = 0;
	// Dont include the checksum bit!!
	for (uint8_t i = 0; i < numBytes - 1; i ++) {
		expectedChecksum += messageData[i];
	}
	
	uint8_t checksum = messageData[numBytes - 1];
	if (expectedChecksum != checksum) {
		Serial.println("Checksum failed");
		Serial.print("Expected: ");
		Serial.println(expectedChecksum);
		Serial.print("Got: ");
		Serial.println(checksum);
		free(messageData);
		waitForMessage = -1;
		return NULL;
	}
	
	waitForMessage = -1;
	return messageData;
	
}

// Unpack messages
uint8_t *UnpackCommandsSerial(uint8_t *messageData) {
	
	uint8_t numCommands = UnpackNumCommandsSerial(messageData);
	
	uint8_t *commandBuffer = (uint8_t*)malloc(numCommands);
	for (uint8_t i = 0; i < numCommands; i ++) {
		commandBuffer[i] = messageData[3 + 2 * i];
	}
	
	return commandBuffer;
	
}
uint8_t *UnpackParametersSerial(uint8_t *messageData) {
	
	uint8_t numCommands = UnpackNumCommandsSerial(messageData);
	
	uint8_t *parameterBuffer = (uint8_t*)malloc(sizeof(uint8_t) * numCommands);
	for (uint8_t i = 0; i < numCommands; i ++) {
		parameterBuffer[i] = messageData[4 + 2 * i];
	}
	
	
	return parameterBuffer;
}
uint8_t UnpackNumCommandsSerial(uint8_t *messageData) {
	return messageData[2];
}


// --------ESPNow methods--------
// Pack messages
uint8_t MessageLengthESPNow(uint8_t numData) {
	return numData * 2;
}
uint8_t *PackMessageESPNow(uint8_t *commandBuffer, uint8_t *parameterBuffer, uint8_t numData) {
	
	uint8_t *messageData = (uint8_t*)malloc(numData * 2);
	for (uint8_t i = 0; i < numData; i ++) {
		messageData[0 + i * 2] = commandBuffer[i];
		messageData[1 + i * 2] = parameterBuffer[i];
	}
	return messageData;
	
}

// Send messages
void SendMessageESPNow(uint8_t *destination, uint8_t *messageData, uint8_t len) {
	esp_now_send(destination, messageData, len);
}
void SendMessageESPNow(uint8_t *destination, uint8_t *commandBuffer, uint8_t *parameterBuffer, uint8_t numData) {
	
	uint8_t *messageData = PackMessageESPNow(commandBuffer, parameterBuffer, numData);
	uint8_t messageLength = numData * 2;
	SendMessageESPNow(destination, messageData, messageLength);
	free(messageData);
	
}

// Read messages
bool ReadReadyESPNow() {
	return false;
}
uint8_t *ReadMessageESPow() {
	return NULL;
}

// Unpack messages
uint8_t *UnpackCommandsESPNow(uint8_t *messageData, uint8_t length) {
	uint8_t *commandBuffer = (uint8_t*)malloc(length);
	for (uint8_t i = 0; i < length; i ++) {
		commandBuffer[i] = messageData[i * 2];
	}
	return commandBuffer;
}
uint8_t *UnpackParametersESPNow(uint8_t *messageData, uint8_t length) {
	uint8_t *parameterBuffer = (uint8_t*)malloc(length);
	for (uint8_t i = 0; i < length; i ++) {
		parameterBuffer[i] = messageData[1 + i * 2];
	}
	return parameterBuffer;
}


// --------Commands--------

// Ping
// Sends a ping to the Serial
void PingSerial(uint8_t sender) {
	
	uint8_t commandBuffer[1] = {0x00};
	uint8_t parameterBuffer[1] = {sender};
	SendMessageSerial(commandBuffer, parameterBuffer, 1);
	
}
void PingESPNow(uint8_t *destination, uint8_t sender) {
	
	uint8_t commandBuffer[1] = {0x00};
	uint8_t parameterBuffer[1] = {sender};
	SendMessageESPNow(destination, commandBuffer, parameterBuffer, 1);
	
}

// Return Ping
// Sends a return ping to the Serial
void ReturnPingSerial(uint8_t sender) {
	
	uint8_t commandBuffer[1] = {0x01};
	uint8_t parameterBuffer[1] = {sender};
	SendMessageSerial(commandBuffer, parameterBuffer, 1);
	
}
void ReturnPingESPNow(uint8_t *destination, uint8_t sender) {
	
	uint8_t commandBuffer[1] = {0x01};
	uint8_t parameterBuffer[1] = {sender};
	SendMessageESPNow(destination, commandBuffer, parameterBuffer, 1);
	
}