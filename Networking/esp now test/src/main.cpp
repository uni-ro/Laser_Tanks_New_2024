#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#define MAC_ADDR_SIZE 6
#define SHOOTER_ID_SIZE 6
//#define DEBUG
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint score = 0;
typedef struct struct_message
{
    char macLocal[MAC_ADDR_SIZE];
    char shooterID[SHOOTER_ID_SIZE];

} struct_message;

struct_message message;

#ifdef DEBUG
byte sendBuffer[255];
byte sendStr[] = {'h', 'e', 'l', 'l', 'o'};
#endif

// Function prototypes
/* Broadcast Function */
void broadcast(const uint8_t *message, int stringLen);
/* RecvCallback Function */
void RecvCallback(uint8_t *senderMAC, uint8_t *incomingData, uint8_t len);
/* SentCallback Function */
void SentCallback(uint8_t *mac_addr, uint8_t sendStatus);
void Freeze();
void Goal();

void setup()
{
    // Init ESP
    Serial.begin(115200);
    Serial.println();
    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
    // Init message struct
    strcpy(message.macLocal, WiFi.macAddress().c_str());
    // Init motor board
    
    // Init ESP-NOW
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_recv_cb(RecvCallback);
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    //

}

void loop()
{
#ifdef DEBUG
    int sendBytes = Serial.available();
    if (sendBytes > 0)
    {
        for (int i = 0; i < 255; i++)
            sendBuffer[i] = 0;
        int writePos = 0;
        while (Serial.available() != 0)
        {
            sendBuffer[writePos] = Serial.read();
            writePos++;
        }
        Serial.print("Broadcasting [");
        for (int i = 0; i < writePos; i++)
        {
            Serial.print(sendBuffer[i]);
            if (i != writePos - 1)
                Serial.print(", ");
        }
        Serial.println("]");
        broadcast(sendBuffer, writePos);
    }
    broadcast(sendStr, 5);
#endif
    // waiting for interruption
    broadcast((uint8_t *)&message, sizeof(message));
    delay(500);
}
void broadcast(const uint8_t *message, int stringLen)
{
    // Broadcast a message to every device in range
    int sendStatus = esp_now_send(broadcastAddress, (u8 *)message, stringLen);
    // Print results to serial monitor
    if (sendStatus == 0)
        Serial.println("Delivery success");
    else
        Serial.println("Delivery fail");
    Serial.println();
}

void RecvCallback(uint8_t *senderMAC, uint8_t *incomingData, uint8_t len)
{
    score++;
#ifdef DEBUG
    Serial.println("Recived from ");
    String senderMAC_s;
    if (sscanf(senderMAC_s.c_str(), "%02X:%02X:%02X:%02X:%02X:%02X", &senderMAC[0], &senderMAC[1], &senderMAC[2], &senderMAC[3], &senderMAC[4], &senderMAC[5]) != 6)
    {
        Serial.println("ERROR while fetch mac address");
    }
    else
    {
        Serial.println(senderMAC_s);
    }
    for (uint8_t i = 0; i < len; i++)
    {
        char tmp = *(incomingData + i);
        Serial.print(tmp);
    }
    Serial.print("\n");
#endif
}

void SentCallback(uint8_t *mac_addr, uint8_t sendStatus)
{

    Serial.print("Last Packet Send Status: ");
    if (sendStatus == 0)
    {
        Serial.println("Delivery success");
    }
    else
    {
        Serial.println("Delivery fail");
    }
}
