#ifndef DRIVE_H
#define DRIVE_H

#include  "ATMEGA2560.h"
#define Drive_Direction_Pin_A 4
#define Drive_Direction_Pin_B 5

#define Drive_Direction_Pin_C 6
#define Drive_Direction_Pin_D 7

void Drive_Init();
void Drive_Update();
void Set_Speed_R(int speed);
void Set_Speed_L(int speed);

#endif