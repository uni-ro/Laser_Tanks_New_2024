#include <Arduino.h>


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
  pinMode(34, INPUT); // X
  pinMode(35, INPUT); // Y
  pinMode(15, INPUT); // Sel

  pinMode(25, INPUT); // X
  pinMode(26, INPUT); // Y
  pinMode(27, INPUT); // Sel

  pinMode(2, OUTPUT);
  
  //pinMode(GPIO_NUM_35, INPUT);
  //pinMode(GPIO_NUM_32, INPUT);
  //pinMode(GPIO_NUM_33, INPUT);
  //put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:

  uint16_t adc_valXL = analogRead(34);
  uint16_t adc_valYL = analogRead(35);
  uint16_t adc_SelL = digitalRead(15);

  uint16_t adc_valXR = analogRead(25);
  uint16_t adc_valYR = analogRead(26);
  uint16_t adc_SelR = digitalRead(27);

  if(adc_SelL == 0)
  {
    digitalWrite(2, 1);
  }
  else
  {
    digitalWrite(2, 0);
  }

  
  
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