#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <cmath>
#include "tank_comms.h"

uint8_t slave_address[] = {0x80, 0x7D, 0x3A, 0x23, 0x7F, 0x58};

const int X = 5;

const int Y = 6;

const int n = 2048;

void test() {
	
	while (true) {
		
		SetMotorSpeed(127, -127);
		delay(1000);
		SetMotorSpeed(-127, 127);
		delay(1000);
		
	}
	
}

void setup() {
  //put your setup code here, to run once:
  Serial.begin(9600);
  
  TankCommsInit(slave_address);


}

void loop() 
{
	// put your main code here, to run repeatedly:

	uint16_t adc_valX = analogRead(X);
	uint16_t adc_valY = analogRead(Y);
	//  uint16_t adc_valYR = analogRead(YR);

	int32_t sX = adc_valX;
	sX -= 2048;
	sX = -127;
	sX /= 2048;

	int32_t sY = adc_valY;
	sY -= 2048;
	sY= -127;
	sY /= 2048;

	//int8_t sX = round((-(n-adc_valX)127)/n);
	//int8_t sY = round(((n-adc_valY)127)/n);

	int32_t L_speed = sY - sX;
	int32_t R_speed = sY + sX;
	L_speed = min(max(-127, L_speed), 127);
	R_speed = min(max(-127, R_speed), 127);

	SetMotorSpeed(L_speed, R_speed);

	//Serial.print("x_input: ");
	//Serial.print(adc_valX);
	//Serial.print(" y_input: ");
	//Serial.println(adc_valY); 

	Serial.print("Left speed: ");
	Serial.print(L_speed);
	Serial.print(" R_speed: ");
	Serial.println(R_speed);

	//Serial.print("sX speed: ");
	//Serial.print(sX);
	//Serial.print(" sY_speed: ");
	//Serial.println(sY);



	delay(10);


}
