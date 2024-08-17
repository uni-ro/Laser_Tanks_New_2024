# Tasks

This document lists everything needed to complete the Laser Tanks project. Feel free to edit this document as you wish, just don't vandalise it.  
Tasks are sorted by priority

## Contents
[Tank CAD Model](https://github.com/curtinrobotics/Laser_Tanks_New_2024/blob/master/Tasks.md#tank-cad-model)
[Source](https://github.com/curtinrobotics/Laser_Tanks_New_2024/tree/master/Tank%20CAD%20Model)  
[Motor Speed Control](https://github.com/curtinrobotics/Laser_Tanks_New_2024/blob/master/Tasks.md#motor-speed-control)
[Source](https://github.com/curtinrobotics/Laser_Tanks_New_2024/tree/master/Motor%20Speed%20Controller)  
[Tank Microcontroller Logic](https://github.com/curtinrobotics/Laser_Tanks_New_2024/blob/master/Tasks.md#tank-microcontroller-logic)
[Source](https://github.com/curtinrobotics/Laser_Tanks_New_2024/tree/master/Tank%20Microcontroller%20Logic)  
[Networking](https://github.com/curtinrobotics/Laser_Tanks_New_2024/blob/master/Tasks.md#networking)
[Source](https://github.com/curtinrobotics/Laser_Tanks_New_2024/tree/master/Networking)  
[Remote Controller](https://github.com/curtinrobotics/Laser_Tanks_New_2024/blob/master/Tasks.md#remote-controller)  
[Laser Detection](https://github.com/curtinrobotics/Laser_Tanks_New_2024/blob/master/Tasks.md#laser-detection)
[Source](https://github.com/curtinrobotics/Laser_Tanks_New_2024/tree/master/Laser%20Receiver)  
[Map Design](https://github.com/curtinrobotics/Laser_Tanks_New_2024/blob/master/Tasks.md#map-design)  
[Powerups](https://github.com/curtinrobotics/Laser_Tanks_New_2024/blob/master/Tasks.md#powerups) 

## Assigning Tasks:

- Each task has a list of members, the members list shows who is working on a specific task.
- If you would like to work on a specific task put your name in the members section so we know who is working on what.
- No need to ask for permission to change what task you are working on.


## Requirements:
- Each task also has a list of requirements.
- This list of requirements serves to guide you on the specifics of what to work on with the tasks.
- Once a requirement has been met, mark it off.
- Requirements should be as specific and explicit as possible.
- If you think of a new requirement, you can add it.


## Tank CAD Model
Members
- Tom
- Silas

Requirements
- Fits the Gearbox and motors
- Fits the tank tracks
- Adjusatable track tightness
- Fits the Microcontroller


## Motor Speed Control  
Members


Requirements
- Specify speed for 2 DC motors (Done)
- Specify direction for 2 DC motors (Done)
- Small circuit


## Tank Microcontroller Logic  
Members


Requirements
- Recieve and parse packets
- Output Motor speed control
- Output laser signal


## Networking  
Members
- Xuan

Requirements
- Packets should identify who the message is for
- Ignore packets not intended for you
- Transmit packets of at least 16 bytes
- Reliable delivery within 30 meters
- Less than 100ms ping


## Remote Controller  
Members


Requirements
- Control for forward / backward motor control
- Control for left / right motor control
- Control for laser firing


## Laser Detection  
Members
- Gabriel
- Silas

Requirements
- Able to detect lasers from +- 90 degrees
- Large target for the lasers to hit
- Transmit serial data over laser?


## Map Design  
Members


Requirements
- Easy to setup and take down
- Fits the tanks

## Powerups
Members


Requirements
- LEDs for signaling what the powerup is and whether it is ready or not
- Communication with the main game server
- Detect when the tank drives over the powerup
- Small and flat, allowing the tanks to drive over it

