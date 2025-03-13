#ifndef __ESP_NOW_UTIL__
#define __ESP_NOW_UTIL__

#define MAC_ADDR_SIZE 12
#define SHOOTER_ID_SIZE 1
#define SIGNCHANNEL 2

#include <Arduino.h>

enum deviceType
{
    TANK,
    CONTROLLER,
    GAMESERVER
};

enum paringStatus
{
    PAIRING,
    PAIRING_SERVER,
    PAIRED_SERVER,
    PAIRING_CONTROLLER,
    PAIRED_CONTROLLER,
    PAIRING_TANK,
    PAIRED_TANK
};

// structure for pairing, the tank itself
typedef struct struct_pairing
{
    enum paringStatus targetStatus = PAIRING;
    deviceType id;
    uint8_t macAddr[6];
} struct_pairing;

typedef struct struct_controller
{
    uint16_t XL,YL,SelL,XR,YR,SelR;
    //uint8_t macAddr[6];
} struct_controller;

typedef struct shooting_info
{
    uint8_t shooterID;
    uint8_t senderMacAddr[6];
} shooting_info;



void GetMacAddress(uint8_t *DistAddress);
void DisplayMAC(uint8_t *MacIndex);
void DisplayMAC(const uint8_t *MacIndex);
void CheckInfo();


#if PLATFORM == 8266

/* Broadcast Function */
void broadcast(const u8 *message, int stringLen);
/* RecvCallback Functions */
void RecvCallbackGaming(uint8_t *senderMAC, uint8_t *incomingData, uint8_t len);
void RecvCallbackPairing(uint8_t *senderMAC, uint8_t *incomingData, uint8_t len);
void RecvCallbackDebugger(uint8_t *senderMAC, uint8_t *incomingData, uint8_t len);
/* SentCallback Function */
void SentCallback(unsigned char *mac_addr, unsigned char sendStatus);
/*report hit to Server*/
void ReportLaserHit(uint8_t shotterID);
/*ParingController or Server*/
void Pairing(paringStatus targetParingStatus);
/*get mac address*/
void GetMacAddress(uint8_t *DistAddress);
/*init espnow*/
int espNowInit();
/*start espnow pairing*/
int espNowStart();
/*Listen ESP Now message and figure out MAC*/
int espNowListener();

#endif

// For esp32
#if PLATFORM == 32
#include <Arduino.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

// broadcast function
void broadcast(const uint8_t *message, int stringLen);
//init espnow, call by Controller
int espNowInitController();
// called by Server/Controller, to start pairing tank(s) 
int espNowStart();
// called by Server /Controller, stop pairing
int espNowStopPairing();
// called by controller, to pair a tank
int espNowStartPairing(paringStatus targetParingStatus);
//RecvCallback Functions
void RecvCallbackPairing(const uint8_t *senderMAC, const uint8_t *incomingData, int len);
// addPeer, called by RecvCallbackPairing
bool addPeer(const uint8_t *peer_addr);
// update movement info manurally
void updateMovementData(uint16_t XL, uint16_t YL, uint16_t SelL, uint16_t XR, uint16_t YR, uint16_t SelR);
// send movement data to a tank
int SendMovementData();
// TODO, to freeze all tanks or defreeze all tanks
void GameManager(bool gameStatus);
// general send-back check
void SentCallback(const uint8_t *mac_addr, esp_now_send_status_t sendStatus);
#endif

#endif
