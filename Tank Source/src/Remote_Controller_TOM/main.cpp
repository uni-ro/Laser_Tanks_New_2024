#include <Arduino.h>

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

}