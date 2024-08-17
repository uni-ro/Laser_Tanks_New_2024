#ifndef PWM_H
#define PWM_H

#include "ATMEGA2560.h"

void PWM_Init_A();
void Set_PWM_Top_A(uint16_t top_val);
void Set_PWM_On_A(uint16_t on_val);

void PWM_Init_B();
void Set_PWM_Top_B(uint16_t top_val);
void Set_PWM_On_B(uint16_t on_val);

#endif