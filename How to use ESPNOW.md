ESPNOW is the communication protocol we are using for the controller / tank communication.
ESPNOWs implementation differs depending on whether you are programming an ESP32 or ESP8266.

## ESPNOW for ESP32
This is how you implement ESPNOW for the ESP32.
### Header files
To use ESPNOW on the ESP32 you need to include the following header files
```cpp title="Required header files for ESPNOW on the ESP32"
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
```

### Initialisation
To set up ESPNOW, use the following code in your setup function
```cpp title="ESPNOW setup code"
// Make sure the ESP32 is in the correct WiFi mode
WiFi.disconnect();
WiFi.mode(WIFI_STA);

uint8 peer_address[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

esp_now_peer_info_t info;
info.channel = 0;
info.encrypt = false;
memcpy(info.peer_addr, slave_address, 6);

esp_now_init();
esp_now_add_peer(&info);
```