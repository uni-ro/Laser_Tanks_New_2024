#ifndef __ESP_NOW_UTIL__
#define __ESP_NOW_UTIL__

#define MAC_ADDR_SIZE 12
#define SHOOTER_ID_SIZE 1
#define SIGNCHANNEL 2
//#define PLATFORM 8266
#define PLATFORM 32
//  general headers
#include <Arduino.h>

uint8_t callbackLock = 0;
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

uint8_t GameStatus = 0;

paringStatus ParingStatus = PAIRING;
struct_controller movementInfo;
struct_pairing paringInfoRec;
struct_pairing paringInfo;
shooting_info laserHit;
uint8_t TankAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t ServerAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t ControlerAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t BroadcastAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
void GetMacAddress(uint8_t *DistAddress)
{
    String val = WiFi.macAddress();
    // Serial.println(val);
    char *endPtr;
    DistAddress[0] = strtol(val.c_str(), &endPtr, 16);
    // read the first starting at the beginning of the buffer. this initializes endPtr as a pointer to the ':' after the first number
    for (int i = 1; (*endPtr) && (i < 6); i++)
        DistAddress[i] = strtol(endPtr + 1, &endPtr, 16);
}

void DisplayMAC(uint8_t *MacIndex)
{
    for (int i = 0; i < 6; i++)
    {
        Serial.print(MacIndex[i], HEX);
        Serial.print(":");
    }
    Serial.println(" Done");
}

void DisplayMAC(const uint8_t *MacIndex)
{
    for (int i = 0; i < 6; i++)
    {
        Serial.print(MacIndex[i], HEX);
        Serial.print(":");
    }
    Serial.println(" Done");
}

void CheckInfo()
{
    Serial.println("TankAddress");
    DisplayMAC(TankAddress);
    Serial.println("ServerAddress");
    DisplayMAC(ServerAddress);
    Serial.println("ControlerAddress");
    DisplayMAC(ControlerAddress);
}

#if PLATFORM == 8266
// This is a tank
#include <ESP8266WiFi.h>
#include <espnow.h>

// Function prototypes
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

int espNowInit()
{
    Serial.print("ESP Board MAC Address:  ");
    WiFi.mode(WIFI_STA);
    wifi_set_channel(SIGNCHANNEL);
    Serial.println(WiFi.macAddress());
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return 0;
    }
    GetMacAddress(TankAddress);
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    paringInfo.id = TANK;
    memcpy(&(paringInfo.macAddr), TankAddress, 6);
    memcpy(&(laserHit.senderMacAddr), TankAddress, 6);
    return 0;
}

int espNowStart()
{
    esp_now_register_send_cb(SentCallback);
    //Pairing(PAIRED_SERVER);
    Pairing(PAIRED_CONTROLLER);
    esp_now_unregister_recv_cb();
    esp_now_register_recv_cb(RecvCallbackGaming);
    return 0;
}

int espNowListener()
{
    esp_now_register_recv_cb(RecvCallbackDebugger);
    return 0;
}

void Pairing(paringStatus targetParingStatus)
{
    paringInfo.targetStatus = targetParingStatus;
    esp_now_register_recv_cb(RecvCallbackPairing);
    // Broadcast the mac address
    while (ParingStatus != targetParingStatus)
    {
        Serial.println("Paring...");
        broadcast((u8 *)&paringInfo, sizeof(paringInfo));
        Serial.println("Paring Info Sent");
        while (callbackLock == 1)
        {
            delay(300);
            Serial.println("Call back is running");
        }
        delay(300);
    }
    esp_now_unregister_recv_cb();
}

void RecvCallbackPairing(uint8_t *senderMAC, uint8_t *incomingData, uint8_t len)
{
    if (callbackLock == 1)
        return;
    callbackLock = 1;
    memcpy(&paringInfoRec, (struct_pairing *)incomingData, sizeof(paringInfoRec));
    Serial.println("Callback is running, locked");
    if (paringInfoRec.id != TANK)
    {
        DisplayMAC(paringInfoRec.macAddr);
        Serial.println("Got pairing information feedback!");
        uint8_t *macPtr;
        if (paringInfoRec.id == CONTROLLER)
        {
            macPtr = ControlerAddress;
            ParingStatus = PAIRED_CONTROLLER;
        }
        else if (paringInfoRec.id == GAMESERVER)
        {
            macPtr = ServerAddress;
        }
        else
        {
            callbackLock = 0;
            return;
        }
        memcpy(macPtr, ((struct_pairing *)incomingData)->macAddr, 6);
        esp_now_add_peer(macPtr, ESP_NOW_ROLE_COMBO, SIGNCHANNEL, NULL, 0);

        // add the controller to the peer list
        bool exists = esp_now_is_peer_exist(macPtr);
        if (exists)
        {
            ParingStatus = (macPtr == ControlerAddress) ? PAIRED_CONTROLLER : PAIRED_SERVER;
        }
        else
        {
        }
    }
    callbackLock = 0;
    Serial.println("Callback finished, unlocked");
}

void RecvCallbackGaming(uint8_t *senderMAC, uint8_t *incomingData, uint8_t len)
{
    DisplayMAC(((struct_pairing *)incomingData)->macAddr);
    switch (len)
    {
    case sizeof(struct_controller):
        Serial.println("controller info from controller ");
        Serial.println("Copying moveinfo");
        memcpy(&movementInfo, incomingData, sizeof(struct_controller));
        break;
    case sizeof(uint8_t):
        if (*incomingData == 1)
        {
            // stop moving
        }
        else if (*incomingData == 0)
        {
            // game start
        }
        break;
    default:
        Serial.println("ERROR");
        break;
    }
}

void RecvCallbackDebugger(uint8_t *senderMAC, uint8_t *incomingData, uint8_t len)
{
    DisplayMAC(senderMAC);
    switch (len)
    {
    case sizeof(struct_controller):
        Serial.println("controller info from controller ");
        Serial.println("Copying moveinfo");
        break;
    case sizeof(struct_pairing):
        Serial.println("pairing info");
        break;
    case sizeof(shooting_info):
        Serial.println("shooting_info");
        break;
    default:
        Serial.println("ERROR");
        break;
    }
}

void ReportLaserHit(uint8_t shooterID)
{
    laserHit.shooterID = shooterID;
    Serial.print("Hit by ");
    // Broadcast a message to every device in range
    int sendStatus = esp_now_send(ServerAddress, (u8 *)&laserHit, sizeof(shooting_info));
    // Print results to serial monitor
    if (sendStatus == 0)
        Serial.println("Delivery success");
    else
        Serial.println("Delivery fail");
    Serial.println();
}

void broadcast(const u8 *message, int stringLen)
{
    // Broadcast a message to every device in range
    int sendStatus = esp_now_send(ServerAddress, (u8 *)message, stringLen);
    // Print results to serial monitor
    if (sendStatus == 0)
        Serial.println("Delivery success");
    else
        Serial.println("Delivery fail");
    Serial.println();
}

void SentCallback(unsigned char *mac_addr, unsigned char sendStatus)
{

    Serial.print("Last Packet Send Status: ");
    if (sendStatus == 0)
        Serial.println("Delivery success");
    else
        Serial.println("Delivery fail");
}

#endif

// For esp32
#if PLATFORM == 32
#include <Arduino.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

esp_now_peer_info_t slave;
int espNowStartPairing(paringStatus targetParingStatus);
void RecvCallbackPairing(const uint8_t *senderMAC, const uint8_t *incomingData, int len);
int espNowStopPairing();

void broadcast(const uint8_t *message, int stringLen)
{
    // Broadcast a message to every device in range
    int sendStatus = esp_now_send(ServerAddress, (uint8_t *)message, stringLen);
    // Print results to serial monitor
    if (sendStatus == 0)
        Serial.println("Delivery success");
    else
        Serial.println("Delivery fail");
    Serial.println();
}

int espNowInitController()
{
    Serial.print("ESP Board MAC Address:  ");
    WiFi.mode(WIFI_STA);
    esp_wifi_set_channel(SIGNCHANNEL, WIFI_SECOND_CHAN_NONE);
    Serial.println(WiFi.macAddress());
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return 0;
    }
    GetMacAddress(ControlerAddress);
    paringInfo.id = CONTROLLER;
    memcpy(&(paringInfo.macAddr), ControlerAddress, 6);
    memcpy(&(laserHit.senderMacAddr), ControlerAddress, 6);
    return 0;
}

int espNowStart()
{
    espNowStartPairing(PAIRED_CONTROLLER);
    esp_now_unregister_recv_cb();
    // esp_now_register_recv_cb(RecvCallbackGaming);
    return 0;
}

bool addPeer(const uint8_t *peer_addr)
{ // add pairing
    Serial.println("Pairing tank");
    memset(&slave, 0, sizeof(slave));
    const esp_now_peer_info_t *peer = &slave;
    memcpy(slave.peer_addr, peer_addr, 6);
    slave.channel = SIGNCHANNEL; // pick a channel
    slave.encrypt = 0;           // no encryption
    bool exists = esp_now_is_peer_exist(slave.peer_addr);
    if (exists)
    {
        Serial.println("Already Paired");
        ParingStatus = PAIRED_TANK;
        return true;
    }
    else
    {
        esp_err_t addStatus = esp_now_add_peer(peer);
        if (addStatus == ESP_OK)
        {
            Serial.println("Pair success");
            ParingStatus = PAIRED_TANK;
            return true;
        }
        else
        {
            Serial.println("Pair failed");
            return false;
        }
    }
}

int espNowStartPairing(paringStatus targetParingStatus)
{
    esp_now_register_recv_cb(RecvCallbackPairing);
    while (ParingStatus != targetParingStatus)
    {
        Serial.println("Finding a tank...");
        delay(300);
    }
    Serial.println("Finded a tank!");
    CheckInfo();
    espNowStopPairing();
    return 0;
}

int espNowStopPairing()
{
    esp_now_unregister_recv_cb();
    return 0;
}

void RecvCallbackPairing(const uint8_t *senderMAC, const uint8_t *incomingData, int len)
{
    DisplayMAC(senderMAC);
    switch (len)
    {
    case sizeof(struct_pairing):
        if (((struct_pairing *)incomingData)->id == TANK)
        {
            Serial.println("Paring info from tank");
            Serial.println("Copying tankinfo");
            memcpy(&paringInfoRec, incomingData, sizeof(struct_pairing));
            memcpy(TankAddress, senderMAC, 6);
            addPeer(paringInfoRec.macAddr);
            Serial.println("Sending response");
            esp_err_t result = esp_now_send(paringInfoRec.macAddr, (uint8_t *)&paringInfo, sizeof(paringInfo));
            ParingStatus = PAIRED_TANK;
        }
        return;
    default:
        Serial.println("Nothing to do");
        return;
    }
}

void updateMovementData(uint16_t XL, uint16_t YL, uint16_t SelL, uint16_t XR, uint16_t YR, uint16_t SelR){
    movementInfo.XL = XL;
    movementInfo.YL = YL;
    movementInfo.SelL = SelL;
    movementInfo.XR = XR;
    movementInfo.YR = YR;
    movementInfo.SelR = SelR;
}

int SendMovementData(){
    return (esp_now_send(0, (uint8_t *)&movementInfo, sizeof(movementInfo)));
}


void GameManager(bool gameStatus)
{
    if (gameStatus)
    {
        GameStatus = 1;
        broadcast(&GameStatus, 1);
    }
    else
    {
        GameStatus = 0;
        broadcast(&GameStatus, 1);
    }
    return;
}
void SentCallback(const uint8_t *mac_addr, esp_now_send_status_t sendStatus)
{

    Serial.print("Last Packet Send Status: ");
    if (sendStatus == 0)
        Serial.println("Delivery success");
    else
        Serial.println("Delivery fail");
}



#endif
#endif
