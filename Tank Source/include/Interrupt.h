#ifndef INTERRUPT_H
#define INTERRUPT_H

#define LOW 0
#define FALLING 1
#define RISING 2
#define CHANGE 3

void AttachInterrupt(int interruptPin, int triggerMode, void (*funPtr)());

#endif