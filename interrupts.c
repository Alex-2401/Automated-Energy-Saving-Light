#include <xc.h>
#include "interrupts.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{
	// turn on global interrupts, peripheral interrupts and the interrupt source 
	// It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.
    PIE0bits.TMR0IE=1; 	//turn on timer Interrupt
                        //INTCON turn on last as they are global
    INTCONbits.GIE=1; 	//turn on interrupts globally (when this is off, all interrupts are deactivated)
    INTCONbits.PEIE = 1; //turn on Peripheral Interrupt
}

/************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
************************************/
void __interrupt(high_priority) HighISR()
{
	//add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    if(PIR0bits.TMR0IF){ 	//check the interrupt source
	    LATHbits.LATH3 = !LATHbits.LATH3;
        //write High reg first, update happens when low reg is written to
        //if both 1:244 and 1:256 iterate through 65535, the time delay is 0.048576s
        //after extensive calculations, the difference of how many bits are iterated through is found to be 3035 between 1 second of 1:244 and 1:256
        //this in binary is 00001011 11011011
        TMR0H=0b00001011;    //setting it to first half (MSBs)
        TMR0L=0b11011011;    //setting it to second half (LSBs)
        //this will happen for every overflow
        PIR0bits.TMR0IF=0; 	//clear the interrupt flag!
	}
}
