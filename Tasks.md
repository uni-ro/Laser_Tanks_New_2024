This document lists everything needed to complete the Laser Tanks project
Feel free to edit this document as you wish, just don't vandalise it.

Assigning Tasks:
Each task has a list of members, the members list shows who is working on a specific task.
If you would like to work on a specific task put your name in the members section so we know who is working on what.
No need to ask for permission to change what task you are working on.

Requirements:
Each task also has a list of requirements.
This list of requirements serves to guide you on the specifics of what to work on with the tasks.
Once a requirement has been met, mark it off with an X in the square brackets e.g
	[X] Finished requirement
	[ ] Unfinished requirement
Requirements should be as specific and explicit as possible.
If you think of a new requirement, you can add it.

Tasks are sorted in terms of priority


1. Tank CAD Model
	Members
		Tom
		Silas
	Requirements
		] Fits the Gearbox and motors
		[ ] Fits the tank tracks
		[ ] Adjusatable track tightness
		[ ] Fits the Microcontroller


2. Motor Speed Control
	Members
	Requirements
		[X] Specify speed for 2 DC motors
		[X] Specify direction for 2 DC motors
		[ ] Small circuit


3. Tank Microcontroller Logic
	Members
	Requirements
		[ ] Recieve and parse packets
		[ ] Output Motor speed control
		[ ] Output laser signal


4. Networking
	Members
		Xuan
	Requirements
		[ ] Packets should identify who the message is for
		[ ] Ignore packets not intended for you
		[ ] Transmit packets of at least 16 bytes
		[ ] Reliable delivery within 30 meters
		[ ] Less than 100ms ping


5. Remote controller
	Members
	Requirements
		[ ] Control for forward / backward motor control
		[ ] Control for left / right motor control
		[ ] Control for laser firing


6. Laser Receiving
	Members
		Gabriel
		Silas
	Requirements
		[ ] Able to detect lasers from +- 90 degrees
		[ ] Large target for the lasers to hit
		[ ] Transmit serial data over laser?


7. Map Design
	Members
	Requirements
		[ ] Easy to setup and take down
		[ ] Fits the tanks

8. Powerups
Members
	Requirements
		[ ] LEDs for signaling what the powerup is and whether it is ready or not
		[ ] Communication with the main game server
		[ ] Detect when the tank drives over the powerup
		[ ] Small and flat, allowing the tanks to drive over it

