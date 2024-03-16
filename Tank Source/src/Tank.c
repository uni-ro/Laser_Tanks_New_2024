#include "ATMEGA2560.h"
#include "Interrupt.h"

bool lightOn = false;

void ButtonPressed() {
	PORTA ^= 1;
}
int main(void) {
	
	DDRA = 0xFF;
	// Cant seem to change the interrupt mode, like wtf???
	AttachInterrupt(INT0, 2, &ButtonPressed);
	
	
	while(true){
		
		PORTA ^= 2;
		_delay_ms(100);
		
		
	};
	
	return 1;
}

