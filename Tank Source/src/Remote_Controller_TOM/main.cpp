#include <Arduino.h>


// put function declarations here:
int myFunction(int, int);

int customMap(int value, int fromLow, int center, int fromHigh, int toLow, int centerOutput, int toHigh) {
  if (value < center) {
    // Map from the lower range
    return map(value, fromLow, center, toLow, centerOutput);
  } else {
    // Map from the upper range
    return map(value, center, fromHigh, centerOutput, toHigh);
  }
}


void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  uint16_t adc_valX = analogRead(GPIO_NUM_32);
  uint16_t adc_valY = analogRead(GPIO_NUM_33);
  int centerX = 2890;
  int centerY = 2890;
  int threshold = 50;

  int scaledX = customMap(adc_valX, 0, 2890, 4095, -100, 0, 100);
  int scaledY = customMap(adc_valY, 0, 2890, 4095, -100, 0, 100);

  if( abs(scaledX) < 5 )
  {
    scaledX = 0;
  }
  
  if( abs(scaledY) < 5 )
  {
    scaledY = 0;
  }

  Serial.print("X: ");
  Serial.print(scaledX);
  Serial.print(' ');
  Serial.print("Y: ");
  Serial.print(scaledY);
  Serial.print("\n");

  delay(50);

}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
