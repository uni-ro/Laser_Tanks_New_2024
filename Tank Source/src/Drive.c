#include "Drive.h"
#include "PWM.h"
#include "ATMEGA2560.h"

void Drive_Init() {
	
	PWM_Init_A();
	PWM_Init_B();
	DDRD |= (1 << Drive_Direction_Pin_A);
	DDRD |= (1 << Drive_Direction_Pin_B);
	DDRD |= (1 << Drive_Direction_Pin_C);
	DDRD |= (1 << Drive_Direction_Pin_D);
	
}
int motor_speed_r = 0;
int motor_speed_desired_r = 0;

int motor_speed_l = 0;
int motor_speed_desired_l = 0;

int motor_acceleration = 60;

void Drive();

void Drive_Update() {
	
	int current_acceleration_r = (motor_speed_desired_r - motor_speed_r);
	if (current_acceleration_r < -motor_acceleration) {current_acceleration_r = -motor_acceleration;}
	if (current_acceleration_r > motor_acceleration) {current_acceleration_r = motor_acceleration;}
	motor_speed_r += current_acceleration_r;
	
	int current_acceleration_l = (motor_speed_desired_l - motor_speed_l);
	if (current_acceleration_l < -motor_acceleration) {current_acceleration_l = -motor_acceleration;}
	if (current_acceleration_l > motor_acceleration) {current_acceleration_l = motor_acceleration;}
	motor_speed_l += current_acceleration_l;
	
	Drive(motor_speed_r, motor_speed_desired_l);
	
	debug_int("L Motor", motor_speed_l);
	debug_int("R Motor", motor_speed_r);
	
}

void Set_Speed_R(int speed) {
	motor_speed_desired_r = speed;
}
void Set_Speed_L(int speed) {
	motor_speed_desired_l = speed;
}
void Drive(int drive_amnt_r, int drive_amnt_l) {
	
	int power_r = abs(drive_amnt_r);
	Set_PWM_On_A(400 - power_r);
	if (drive_amnt_r < 0) {
		PORTA |= (1 << Drive_Direction_Pin_A);
		PORTA &= ~(1 << Drive_Direction_Pin_B);
	}
	else {
		PORTA &= ~(1 << Drive_Direction_Pin_A);
		PORTA |= (1 << Drive_Direction_Pin_B);
	}
	
	int power_l = abs(drive_amnt_l);
	Set_PWM_On_B(400 - power_l);
	if (drive_amnt_l < 0) {
		PORTA |= (1 << Drive_Direction_Pin_C);
		PORTA &= ~(1 << Drive_Direction_Pin_D);
	}
	else {
		PORTA &= ~(1 << Drive_Direction_Pin_C);
		PORTA |= (1 << Drive_Direction_Pin_D);
	}
	
}