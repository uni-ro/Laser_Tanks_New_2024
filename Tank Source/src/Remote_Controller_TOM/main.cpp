#include <Arduino.h>

uint8_t slave_address[] = {0x80, 0x7D, 0x3A, 0x23, 0x7F, 0x58};

const int XL = 14;
const int YL = 27;

const int XR = 26;
const int YR = 25;

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