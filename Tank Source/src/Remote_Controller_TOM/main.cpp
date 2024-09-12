#include <Arduino.h>


int customMap(int value, int fromLow, int center, int fromHigh, int toLow, int centerOutput, int toHigh) {
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
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:

  uint16_t adc_valX = analogRead(GPIO_NUM_32);
  uint16_t adc_valY = analogRead(GPIO_NUM_33);
  uint16_t adc_valSel = digitalRead(GPIO_NUM_35);
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

  // Printing
  Serial.print(">X:"); 
  Serial.print(scaledX);
  Serial.print(',');
  Serial.print("Y:");
  Serial.print(scaledY);
  Serial.print(',');
  Serial.print("Sel:");
  Serial.print(adc_valSel);
  Serial.print("\n");

  delay(50);
}