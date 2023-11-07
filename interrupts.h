#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000

//Define Pins/LEDs Used as flags
#define LDR_FLAG LATDbits.LATD7   
#define TIMER_FLAG LATDbits.LATD4    

void Interrupts_init(void);
void __interrupt(high_priority) HighISR();
void __interrupt(low_priority) LowISR();

#endif
