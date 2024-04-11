#include "Drive.h"
#include "ATMEGA2560.h"

uint16_t timer_top_val = 400; // This is the where the timer will hit the top and reset, it will decide the frequency
uint16_t timer_on_val = 200; // This is where the on the timer the pwm will switch on. This will decide the duty cycle

void Drive_Init() {
	
	DDRB |= (1 << 5);
	
	// COM1A1 COM1A0 COM1B1 COM1B0 COM1C1 COM1C0 WGM11 WGM10
	// P.154
	TCCR1A = 0b11000010;
	// ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
	// P.156
	TCCR1B = 0b00011010;
	
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
	//CS11 = 0
	//CS10 = 1
	// Prescaler is 1

	OCR1AH = (timer_on_val >> 8);
	OCR1AL = timer_on_val;
	
	ICR1H = (timer_top_val >> 8);
	ICR1L = timer_top_val;
	
}
void Drive_Update() {
	int potAmnt = (float)adc_read(PORTF7) / 1023.0 * 400.0;
	debug_int("Pot amnt", potAmnt);
	Set_PWM_Duty(potAmnt);
}
void Set_PWM_Duty(uint16_t amnt) {
	
	timer_on_val = amnt;
	OCR1AH = (timer_on_val >> 8);
	OCR1AL = timer_on_val;
	
}