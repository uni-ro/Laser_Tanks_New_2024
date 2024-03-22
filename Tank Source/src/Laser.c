// This file controls the Laser system
// Author: Daniel Denison

// Settings:
//		LASER_DURATION	How many milliseconds the laser should fire for, before turning off automatically
//		LASER_PIN		What pin the laser is plugged into, assumed in PORTA

// Controls:
//		LaserShoot() will shoot the laser, this will automatically handle the timing to turn the laser off

// Engine:
//		LaserInit() to setup the laser. This will set the required pin direction for the pin specified by LASER_PIN
//		LaserUpdate() to allow this file to carry out its operations

#include "ATMEGA2560.h"
#include "Laser.h"
#include "Settings.h"
#include "serial.h"

uint32_t laser_on_time = 0;
bool laser_on = false; 

void priv_LaserOff();
void priv_LaserOn();

// Perform all necessary setup for the laser
void Laser_Init() {
	DDRA |= (1 << LASER_PIN); // Make sure the pin the laser is connected to is designated as an output pin
	priv_LaserOff(); // Start with the laser off
}

// Continuous loop
void Laser_Update() {
	// If the laser has been on for longer than its duration, turn it off
	if (laser_on == true && ms > (laser_on_time + LASER_DURATION)) {
		laser_on_time = 0;
		priv_LaserOff();
	}
}

// Shoots the laser
void Laser_Shoot() {
	laser_on_time = ms; // Records what the current time is, so we know when to turn it off
	priv_LaserOn(); // Turns the laser on
}

// Handles turning the laser off
void priv_LaserOff() {
	if (laser_on == false) { return; } // If the laser is already off, do nothing
	#ifdef DEUBG_LASER == 1 // Only include the debug code if debugging is enabled
	debug_string("laser_off"); // Debug turning the laser off
	#endif
	laser_on = false;
	PORTA &= ~(1 << LASER_PIN); // Set the pin connected to the laser to low
}

// Handles turning the laser on
void priv_LaserOn() {
	if (laser_on == true) { return; } // If the laser is alread on, do nothing
	#ifdef DEBUG_LASER == 1 // Only include the debug code if debugging is enabled
	debug_string("laser_on"); // Debug turning the laser on
	#endif
	laser_on = true;
	PORTA |= (1 << LASER_PIN); // Set the pin connected to the laser to high
}