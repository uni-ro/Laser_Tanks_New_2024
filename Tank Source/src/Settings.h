// TODO: Add comments
#include "ATMEGA2560.h"

#ifndef SETTINGS_H
#define SETTINGS_H

// Debug settings
#define DEBUG_LASER 0
#define DEBUG_COMMS_RECIEVED 1


// Laser settings
#define LASER_DURATION 10000000000 // 100 ms or 0.1 seconds
#define LASER_PIN 3 // Pin 3 on PORTA

// Comms settings
#define COMMS_PIN 0 // Tx and Rx 0

uint32_t ms;

#endif