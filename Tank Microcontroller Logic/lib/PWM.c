#include "PWM.h"
#include "ATMEGA2560.h"

uint16_t timer_top_val = 400; // This is the where the timer will hit the top and reset. This will decide the frequency
uint16_t timer_on_val = 200; // This is where the on the timer the pwm will switch on. This will decide the duty cycle

void PWM_Init_A() {
	
	DDRB |= (1 << 5);
	
	// COM1A1 COM1A0 COM1B1 COM1B0 COM1C1 COM1C0 WGM11 WGM10
	// P.154
	TCCR1A = 0b11000010;
	// ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
	// P.156
	TCCR1B = 0b00011011;
	
	// COM1A1 = 1
	// COM1A0 = 1
	// Set PWM to high on going up and low on reset.
	// P.155
	
	// WGM13 = 1
	// WGM12 = 1
	// WGM11 = 0
	// WGM10 = 0
	// CTC mode with top as ICR1
	// P.145
	
	//CS12 = 0
	//CS11 = 1
	//CS10 = 1
	// Prescaler is 64

	// For 16 bit registers, you must set the high bits first P.135
	OCR1AH = (timer_on_val >> 8);	// Load on value into the 16 bit register's high bits
	OCR1AL = timer_on_val;			// Load on value into the 16 bit register's low bits
	
	ICR1H = (timer_top_val >> 8);	// Load top value into the 16 bit register's high bits
	ICR1L = timer_top_val;			// Load top value into the 16 bit register's low bits
	
}
void Set_PWM_Top_A(uint16_t top_val) {
	// Repeat of what happens in PWM_Init();
	timer_top_val = top_val;
	ICR1H = (timer_top_val >> 8);
	ICR1L = timer_top_val;
}
void Set_PWM_On_A(uint16_t on_val) {
	// Repeat of what happens in PWM_Init();
	timer_on_val = on_val;
	if (timer_on_val > timer_top_val - 1) {
		timer_on_val = timer_top_val - 1;
	}
	if (timer_on_val < 0) {
		timer_on_val = 0;
	}
	OCR1AH = (timer_on_val >> 8);
	OCR1AL = timer_on_val;
}

void PWM_Init_B() {
	
	DDRE |= (1 << 3);
	
	// COM1A1 COM1A0 COM1B1 COM1B0 COM1C1 COM1C0 WGM11 WGM10
	// P.154
	TCCR3A = 0b11000010;
	// ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
	// P.156
	TCCR3B = 0b00011011;
	
	// COM1A1 = 1
	// COM1A0 = 1
	// Set PWM to high on going up and low on reset.
	// P.155
	
	// WGM13 = 1
	// WGM12 = 1
	// WGM11 = 0
	// WGM10 = 0
	// CTC mode with top as ICR1
	// P.145
	
	//CS12 = 0
	//CS11 = 1
	//CS10 = 1
	// Prescaler is 64

	// For 16 bit registers, you must set the high bits first P.135
	OCR3AH = (timer_on_val >> 8);	// Load on value into the 16 bit register's high bits
	OCR3AL = timer_on_val;			// Load on value into the 16 bit register's low bits
	
	ICR3H = (timer_top_val >> 8);	// Load top value into the 16 bit register's high bits
	ICR3L = timer_top_val;			// Load top value into the 16 bit register's low bits
	
}
void Set_PWM_Top_B(uint16_t top_val) {
	// Repeat of what happens in PWM_Init();
	timer_top_val = top_val;
	ICR3H = (timer_top_val >> 8);
	ICR3L = timer_top_val;
}
void Set_PWM_On_B(uint16_t on_val) {
	// Repeat of what happens in PWM_Init();
	timer_on_val = on_val;
	OCR3AH = (timer_on_val >> 8);
	OCR3AL = timer_on_val;
}