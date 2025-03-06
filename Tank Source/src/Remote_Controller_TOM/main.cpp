#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

uint8_t slave_address[] = {0x80, 0x7D, 0x3A, 0x23, 0x7F, 0x58};

const int XL = 32;

const int XR = 33;


int customMap(int value, int fromLow, int center, int fromHigh, int toLow, int centerOutput, int toHigh)
{
  if (value < center) {
    // Map from the lower range
    return map(value, fromLow, center, toLow, centerOutput);
  } else {
    // Map from the upper range
    return map(value, center, fromHigh, centerOutput, toHigh);
  }
}

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

  
  
  uint16_t adc_valXL = analogRead(XL);
  uint16_t adc_valXR = analogRead(XR);

  uint8_t sendXL;
  uint8_t sendXR;

  if( adc_valXL < 1800 )
  {
    sendXL = 0x3F;
  }
  else if( adc_valXL > 2200 )
  {
    sendXL = 0x7F;
  }
  else
  {
    sendXL = 0;
  }

  if( adc_valXR < 1800 )
  {
    sendXR = 0x3F;
  }
  else if( adc_valXR > 2200 )
  {
    sendXR = 0x7F;
  }
  else
  {
    sendXR = 0;
  }

  uint8_t leftmotor[] = {0x01, sendXL};
  uint8_t rightmotor[] = {0x02, sendXR};
  
  esp_now_send(slave_address, leftmotor, 2);
  esp_now_send(slave_address, rightmotor, 2);
  char printStr[255];
  
  sprintf(printStr, "(%04D, %04D) ", adc_valXL, adc_valXR);
  Serial.print(printStr);
  Serial.println();

  sprintf(printStr, "(%04X, %04X) ", sendXL, sendXR);
  Serial.print(printStr);
  Serial.println();
  

  delay(50);

}