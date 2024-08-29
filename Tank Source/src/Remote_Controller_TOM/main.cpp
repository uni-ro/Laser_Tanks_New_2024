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

  uint16_t adc_val1 = analogRead(GPIO_NUM_32);
  uint16_t adc_val2 = analogRead(GPIO_NUM_33);
  Serial.print("X: ");
  Serial.print(adc_val1);
  Serial.print(' ');
  Serial.print("Y: ");
  Serial.print(adc_val2);
  Serial.print("\n");

  delay(50);

}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
