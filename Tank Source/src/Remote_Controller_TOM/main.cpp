#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

uint8_t slave_address[] = {0x80, 0x7D, 0x3A, 0x23, 0x7F, 0x58};

const int XL = 14;
const int YL = 27;

const int XR = 26;
const int YR = 25;


void InitESPNow() {
	
	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	
	esp_now_init();
	
	esp_now_peer_info_t info;
	info.channel = 0;
	info.encrypt = false;
	memcpy(info.peer_addr, slave_address, 6);
	esp_now_add_peer(&info);
	
}

void setup() 
{
  //put your setup code here, to run once:
  Serial.begin(9600);
  InitESPNow();
  //pinMode(XL, INPUT); // XL
  //pinMode(YL, INPUT); // YL
  //pinMode(SelL, INPUT); // SelL

  //pinMode(XR, INPUT); // XR
  //pinMode(YR, INPUT); // YR
  //pinMode(SelR, INPUT); // SelR
  
  
}

void loop() 
{
  // put your main code here, to run repeatedly:

  
  uint8_t data[] = {0x00, 0x00};
  esp_now_send(slave_address, data, 2);
  
  uint16_t adc_valXL = analogRead(XL);
  uint16_t adc_valYL = analogRead(YL);
  
  uint16_t adc_valXR = analogRead(XR);
  uint16_t adc_valYR = analogRead(YR);
  
  char printStr[255];
  
  sprintf(printStr, "(%04D, %04D) ", adc_valXL, adc_valYL);
  Serial.print(printStr);
  
  sprintf(printStr, "(%04D, %04D)", adc_valXR, adc_valYR);
  Serial.println(printStr);
  

  delay(50);

}