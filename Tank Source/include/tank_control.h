#ifndef TANK_CONTROL_H
#define TANK_CONTROL_H

#include <Arduino.h>>

#define LEFT_MOTOR_PWM 14
#define LEFT_MOTOR_DIR_A 4
#define LEFT_MOTOR_DIR_B 5

#define RIGHT_MOTOR_PWM 15
#define RIGHT_MOTOR_DIR_A 13
#define RIGHT_MOTOR_DIR_B 12

#define LIGHT_SENSOR A0

void SetLeftMotorSpeed(uint8_t speed, uint8_t direction) {
	
	analogWrite(LEFT_MOTOR_PWM, speed);
	digitalWrite(LEFT_MOTOR_DIR_A, direction ^ 1);
	digitalWrite(LEFT_MOTOR_DIR_B, direction);
	
}
void SetRightMotorSpeed(uint8_t speed, uint8_t direction) {
	
	analogWrite(RIGHT_MOTOR_PWM, speed);
	digitalWrite(RIGHT_MOTOR_DIR_A, direction ^ 1);
	digitalWrite(RIGHT_MOTOR_DIR_B, direction);
	
}

#endif