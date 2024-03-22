// How it works
// Think of the tank as a bunch of systems connected by this main function
// The systems could be something like:
//		Laser system
//		Movement system
//		Communication system

// Each system has a couple of different components
//		Settings, which are defined in Settings.h. Pretty self explainatory

//		Controls, which will be defined in their respective header files. Controls are what you use to get the systems to do something,
//		wether it be shooting a laser, driving, changing direction or whatever. The controls must be set up in such a way that no matter
//		how you invoke them they will never cause bugs or unexpected behaviour.

//		Engine, engine related stuff. This would include initialisation and the main update for each system to "think". In the case
//		of the laser system, the initialisation is in LaserInit() and sets the pin directions and give its variables initial values
//		and the main update for the laser system is deciding when the laser has been on for too long and turning it off

//		Private functions, These help the systems do whatever they need to do but are only to be invoked by the system itself. For example
//		in the laser system, it has the priv_LaserOn() for turning the laser on. If this is invoked then none of the timer logic will be
//		initiated so the laser will be on forever. The private functions arent defined in the header files so this should be hard to mess up

// Rules for never causing bugs
// 1. Make sure each system's controls can be invoked in whatever way without causing bugs
// 2. Never refrence a system's variables from outside the system. If you need to get data back from it, make a "control" that returns it
// 3. TBD

#include "Settings.h"
#include "ATMEGA2560.h"
#include "Comms.h"
#include "Laser.h"

int main() {
	
	Laser_Init(); // Setup the Laser system
	Comms_Init(); // Setup the Comms system
	
	milliseconds_init(); // Start the timer
	
	while (true) {
		ms = milliseconds_now(); // Record the current time
		
		Laser_Update(); // Laser system "thinking"
		Comms_Update(); // Comms system "thinking"
		
		_delay_ms(1); // Little pause
		
	}
	
	return 1;
}

