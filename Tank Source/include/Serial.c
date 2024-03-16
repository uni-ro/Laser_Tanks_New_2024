/*
#include "atmega2560.h"

void SendUARTByte(char byte) {
	
	DDRE = 0b00000010;
	
	char sendByte = byte;
	short serialData = 0;
	
	//Start bit, pull TX low for start of data transmission
	serialData = ((serialData >> 1) | (0 << 10));
	
	//Data Frame
	int numOnes = 0;
	for (int i = 0; i < 8; i ++) {
		serialData = ((serialData >> 1) | ((sendByte & 1) << 10));
		
		if ((sendByte & 1) == 1) {
			numOnes ++;
		}
		sendByte >>= 1;
		
	}
	
	//Parity bit
	serialData = ((serialData >> 1) | ((numOnes % 2) << 10));
	
	//Stop bit
	serialData = ((serialData >> 1) | (1 << 10));
	

	//--Sending Data--
	for (int i = 0; i < 11; i ++) {
		PORTE = ((serialData >> i) & 1) << 1;
	}
	
}
*/