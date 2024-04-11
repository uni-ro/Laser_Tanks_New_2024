// TODO: Add comments

#include "ATMEGA2560.h"
#include "Settings.h"
#include "Laser.h"
#include "serial.h"

void priv_Recieved_Byte(uint8_t byte);

void Comms_Init() {
	Serial_Init(COMMS_PIN);
}
void Comms_Update() {
	
	if (Serial_Byte_Ready(COMMS_PIN) == true) {
		
		while (Serial_Byte_Ready(COMMS_PIN) == true) {
			priv_Recieved_Byte(Serial_Read_Byte(COMMS_PIN));
		}
		
	}
	
}
void priv_Recieved_Byte(uint8_t byte) {
	
	#if DEBUG_COMMS_RECIEVED == 1
	debug_int("Byte recieved", byte);
	#endif
	
	if (byte == (int)'F') {
		Laser_Shoot();
	}
}