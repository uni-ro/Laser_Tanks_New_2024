#ifndef COMMST_H
#define COMMST_H

#include <Arduino.h>

// Comms
// Start byte A - 0xFF
// Start byte B - 0x7F
// Num data

// Info header
// Data

// Info header
// Data

// ...

// Info header
// Data

// Checksum (sum of all values, looped within 255)

// The info header contains info about both where the message came from as well as what the message is
// The first 5 bytes describes what the info is (least significant)
// The last 3 describe the sender(most significant)

// Info headers
// 0 - ping
// 1 - ping return
// 2 - Left motor speed
// 3 - Right motor speed

// Senders
// 0 - PC
// 1 - Master ESP
// 2 - Slave ESP

void Comms_Init();

// --------Serial methods--------
// Pack messages
uint8_t MessageLengthSerial(uint8_t numData);
uint8_t *PackMessageSerial(uint8_t *commandBuffer, uint8_t *parameterBuffer, uint8_t numData);

// Send messages
void SendMessageSerial(uint8_t *messageData, uint8_t len);
void SendMessageSerial(uint8_t *commandBuffer, uint8_t *parameterBuffer, uint8_t numData);

// Read messages
bool ReadReadySerial();
uint8_t *ReadMessageSerial();

// Unpack messages
uint8_t *UnpackSenderSerial(uint8_t *messageData);
uint8_t *UnpackCommandsSerial(uint8_t *messageData);
uint8_t *UnpackParametersSerial(uint8_t *messageData);
uint8_t UnpackNumCommandsSerial(uint8_t *messageData);


// --------ESPNow methods--------
// Pack messages
uint8_t MessageLengthESPNow(uint8_t numData);
uint8_t *PackMessageESPNow(uint8_t *commandBuffer, uint8_t *parameterBuffer, uint8_t numData);

// Send messages
void SendMessageESPNow(uint8_t *messageData, uint8_t len);
void SendMessageESPNow(uint8_t *destination, uint8_t *commandBuffer, uint8_t *parameterBuffer, uint8_t numData);

// Read messages
bool ReadReadyESPNow();
uint8_t *ReadMessageESPNow();

// Unpack messages
uint8_t *UnpackSenderESPNow(uint8_t *messageData, uint8_t length);
uint8_t *UnpackCommandsESPNow(uint8_t *messageData, uint8_t length);
uint8_t *UnpackParametersESPNow(uint8_t *messageData, uint8_t length);


// --------Commands--------
void PingSerial(uint8_t sender);
void PingESPNow(uint8_t *destination, uint8_t sender);

void ReturnPingSerial(uint8_t sender);
void ReturnPingESPNow(uint8_t *destination, uint8_t sender);


#endif