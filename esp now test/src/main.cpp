#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// put function declarations here:
typedef struct struct_message
{
    char a[32];

} struct_message;

// Create a struct_message called shotted
struct_message shotted;

// Emulates a broadcast
void broadcast(const String &message)
{
    // Broadcast a message to every device in range
    // Send to all
    int sendStatus = esp_now_send(broadcastAddress, (u8 *)message.c_str(), message.length());
    // Print results to serial monitor
    if (sendStatus == 0)
    {
        Serial.println("Delivery success");
    }
    else
    {
        Serial.println("Delivery fail");
    }
    Serial.println();
}

void RecvCallback(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
    Serial.print("Recived");
    Serial.println(*mac);
}
/*
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
*/
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println();
    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    // esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_recv_cb(RecvCallback);
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    broadcast("shotted by ");
}

void loop()
{
    // put your main code here, to run repeatedly:
}
