#ifndef DRIVE_H
#define DRIVE_H

#include  "ATMEGA2560.h"

void Drive_Init();
void Drive_Update();
void Set_PWM_Duty(uint16_t amnt);

#endif