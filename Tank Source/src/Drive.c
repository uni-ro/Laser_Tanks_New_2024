#include "Drive.h"
#include "PWM.h"
#include "ATMEGA2560.h"

void Drive_Init() {
	
	PWM_Init_A();
	
}
int amnt = 0;
int dir = 5;
void Drive_Update() {
	amnt += dir;
	if (amnt > 400 - 1) {
		dir *= -1;
	} 
	if (amnt < 0) {
		dir *= -1;
	} 
	debug_int("Drive amnt", amnt);
	Set_PWM_On_A(amnt);
}