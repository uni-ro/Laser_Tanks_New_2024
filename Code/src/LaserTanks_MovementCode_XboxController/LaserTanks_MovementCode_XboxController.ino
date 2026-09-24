// Select Esp32 Dev Module - esp32_bluepad32 for the board when uploading

#include <Bluepad32.h>

ControllerPtr myController = nullptr;

// TA6586 Control pins
//The Motor driver is a Half H-Brdige, with to input pins
//Having one high and the other lower will cause the motor to spin one way
//Switch polarity to change directions
//When both are high the motor will brake **
//When both are Low the motor will coast ** 
const int BI_L = 14;
const int FI_L = 27;
const int BI_R = 26;
const int FI_R = 25;

//Intitalising values
int leftPWM = 0;
int rightPWM = 0;
int throttle = 0;
int turn = 0;


void onConnectedController(ControllerPtr ctl) {
  if (myController == nullptr) {
    Serial.print("CALLBACK: Controller is connected, index=");
    Serial.println(ctl->index());
    // Make sure motors stop when connected
    digitalWrite(FI_L, 0);
    digitalWrite(FI_R, 0);
    digitalWrite(BI_L, 0);
    digitalWrite(BI_R, 0);
    myController = ctl;
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  if (myController == ctl) {
    Serial.print("CALLBACK: Controller is disconnected from index=");
    Serial.println(ctl->index());
    // Stop motors when controller disconnects
    digitalWrite(FI_L, 0);
    digitalWrite(FI_R, 0);
    digitalWrite(BI_L, 0);
    digitalWrite(BI_R, 0);
    myController = nullptr;
  }
}

void setup() {
  Serial.begin(115200);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
  //Intitalising Pins
  pinMode(FI_L, OUTPUT);
  pinMode(FI_R, OUTPUT);
  pinMode(BI_L, OUTPUT);
  pinMode(BI_R, OUTPUT);
}


void loop() {
  BP32.update();
  if (myController && myController->isConnected()) {// This was orignally if(myController && myController->isConnected && myControllerhasUpdate)but the update do not send often enough and reading can be off if you move controller to fast
    
    turn = (myController->axisX())/2; // converts the axisX signed 9 bit (512) reading into a signed 8 bit(256) reading
    throttle = -(myController->axisY())/2; // Does same as axisX but the Y axis from the controller has the top as negtive so it need to be inverted
    // Converting controller readings to PWM values for the tank stear using two motors
    leftPWM = throttle + turn;
    rightPWM = throttle - turn;
    leftPWM = constrain(leftPWM, -255, 255);
    rightPWM = constrain(rightPWM, -255, 255);

//Send PWM values to the correct pin on motor drive, have had trouble with having this as a sepperate function that is called but I am not sure if i was just making an error
//Right Motor    
    if (abs(rightPWM)>50){//Checks to see if in dead zone
      if (rightPWM >= 0) { // since value > 0 motor need to go forwards so pwm value is send to the forwards pin
          analogWrite(FI_R, rightPWM);
          analogWrite(BI_R, 0); // using digitalWrite(Pin, LOW) here causes problems for some reason so keep it as an analogWrite(Pin, 0)
          //Serial.print("Ford: ");
          //Serial.println(rightPWM);
          } 
      else {//since its not value > 0 motor needs to be driven backwards so pwm send to the backwards pin
        analogWrite(BI_R, -rightPWM); // makes Pwm value positive
        analogWrite(FI_R, 0);
        //Serial.print("Back: ");
        //Serial.println(-rightPWM);
        }}
    else{ //if in dead zone set both pins to low
      analogWrite(BI_R,0);
      analogWrite(FI_R,0);
      //Serial.println("low");
      }

//Left Motor
    if (abs(leftPWM)>50){
      if (leftPWM >= 0) {
          analogWrite(FI_L, leftPWM);
          analogWrite(BI_L, 0);
          //Serial.print("Ford: ");
          //Serial.println(leftPWM);
          } 
      else {
        analogWrite(BI_L, -leftPWM);
        analogWrite(FI_L, 0);
        //Serial.print("Back: ");
        //Serial.println(-leftPWM);
        }}
    else{
      analogWrite(BI_L,0);
      analogWrite(FI_L,0);
      //Serial.println("low");
      }
  }
}