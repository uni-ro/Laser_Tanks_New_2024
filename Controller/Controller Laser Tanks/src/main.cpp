#include <Arduino.h>

char string[256];


// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  uint16_t adc_val = analogRead(GPIO_NUM_32);
  Serial.print(adc_val);
  Serial.print("\n");

  delay(50);

}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}