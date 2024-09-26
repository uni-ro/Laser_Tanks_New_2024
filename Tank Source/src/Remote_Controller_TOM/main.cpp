#include <Arduino.h>
#include <espnow.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include "commst.h"

uint8_t slave_address[] = {0x80, 0x7D, 0x3A, 0x23, 0x7F, 0x58};

const int XL = 35;
const int YL = 34;
const int SelL = 32;
const int XR = 25;
const int YR = 33;
const int SelR = 26;

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


void setup() 
{
  //put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(XL, INPUT); // XL
  pinMode(YL, INPUT); // YL
  pinMode(SelL, INPUT); // SelL

  pinMode(XR, INPUT); // XR
  pinMode(YR, INPUT); // YR
  pinMode(SelR, INPUT); // SelR

  //pinMode(2, OUTPUT);
  Serial.println();
	Serial.println();
	Serial.println("This board is the MASTER");
	Serial.print("MAC Address: ");
	Serial.println(WiFi.macAddress());
	
	InitESPNow();
	
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
	
	Comms_Init();
}

void loop() 
{
  // put your main code here, to run repeatedly:

  uint16_t adc_valXL = analogRead(XL);
  uint16_t adc_valYL = analogRead(YL);
  uint16_t adc_SelL = digitalRead(SelL);

  uint16_t adc_valXR = analogRead(XR);
  uint16_t adc_valYR = analogRead(YR);
  uint16_t adc_SelR = digitalRead(SelR);

  /*if(adc_SelL == 0)
  {
    digitalWrite(2, 1);
  }
  else
  {
    digitalWrite(2, 0);
  }*/

  
  
  //uint16_t adc_valSel = digitalRead(GPIO_NUM_15);
  
  /*
  int centerX = 2890;
  int centerY = 2890;
  int threshold = 50;

  int scaledX = customMap(adc_valX, 0, 2890, 4095, -100, 0, 100); // Mapping to -100 to 100
  int scaledY = customMap(adc_valY, 0, 2890, 4095, -100, 0, 100); // Mapping to -100 to 100

  if( abs(scaledX) < 5 ) // Removing noise at center
  {
    scaledX = 0;
  }
  
  if( abs(scaledY) < 5 ) // Removing noise center
  {
    scaledY = 0;
  }
  */

  // Printing
  Serial.print(">XL:"); 
  Serial.print(adc_valXL);
  Serial.print(',');
  Serial.print("YL:");
  Serial.print(adc_valYL);
  Serial.print(',');
  Serial.print("SelL:");
  Serial.print(adc_SelL);
  Serial.print(">XR:"); 
  Serial.print(adc_valXR);
  Serial.print(',');
  Serial.print("YR:");
  Serial.print(adc_valYR);
  Serial.print(',');
  Serial.print("SelR:");
  Serial.print(adc_SelR);
  Serial.println();

  delay(50);
	
	
	if (ReadReadySerial() == true) {
		
		uint8_t *messageData = ReadMessageSerial();
		if (messageData != NULL) {
			SerialCallback(messageData);
			free(messageData);
		}
		
		
	}
}


void HandleCommand(uint8_t command, uint8_t parameter) {
	// Senders
	// 0 - PC
	// 1 - Master ESP
	// 2 - Slave ESP
	
	if (command == 0x00) {
		// Ping command
		
		if (parameter == 0) {
			ReturnPingSerial(1);
		}
		else {
			ReturnPingESPNow(slave_address, 1);
		}
		
	}
	
}
void ESPNowCallback(uint8_t *mac_addr, uint8_t *data, uint8_t length) {
	
	uint8_t *commandBuffer = UnpackCommandsESPNow(data, length);
	uint8_t *parameterBuffer = UnpackParametersESPNow(data, length);
	
	for (uint8_t i = 0; i < length / 2; i ++) {
		HandleCommand(commandBuffer[i], parameterBuffer[i]);
	}
	// Relay the message
	SendMessageSerial(commandBuffer, parameterBuffer, length / 2);
	
	free(commandBuffer);
	free(parameterBuffer);
	
}
void SerialCallback(uint8_t *messageData) {
	
	uint8_t numCommands = UnpackNumCommandsSerial(messageData);
	uint8_t *commandBuffer = UnpackCommandsSerial(messageData);
	uint8_t *parameterBuffer = UnpackParametersSerial(messageData);

	for (uint8_t i = 0; i < numCommands; i ++) {
		HandleCommand(commandBuffer[i], parameterBuffer[i]);
	}
	SendMessageESPNow(slave_address, commandBuffer, parameterBuffer, numCommands);
	
	free(commandBuffer);
	free(parameterBuffer);
	
}



void InitESPNow() {
	
	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	if (esp_now_init() == 0) {
		Serial.println("Success initialising ESPNow");
	}
	else {
		Serial.println("Failed to initialise ESPNow");
		Serial.println("Retrying in 3 seconds");
		delay(3000);
		InitESPNow();
	}
	
	// ESPNow has initialised successfully here
	esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
	esp_now_add_peer(slave_address, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
	esp_now_register_recv_cb(ESPNowCallback);
		
}