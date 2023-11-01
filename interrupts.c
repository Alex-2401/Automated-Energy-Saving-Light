#include <xc.h>
#include "interrupts.h"

volatile unsigned int hour = 0;
/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{
	// turn on global interrupts, peripheral interrupts and the interrupt source 
	// It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.
    PIE0bits.TMR0IE=1; 	//turn on timer Interrupt
    PIE2bits.C1IE=1; 	//turn on Comparator 1 Interrupt
                        //INTCON turn on last as they are global
    IPR2bits.C1IP = 1; //set high priority for comparator
    IPR0bits.TMR0IP = 0; //set low priority for timer
    INTCONbits.IPEN=1;  //interrupt priority setting (enabled)
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
    if(PIR2bits.C1IF){ 	//check the interrupt source
	    LATHbits.LATH3 = !LATHbits.LATH3;
        PIR2bits.C1IF=0; 	//clear the interrupt flag!       
    }
}

void __interrupt(low_priority) LowISR()
{
    //add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    if(PIR0bits.TMR0IF){ 	//check the interrupt source
        TMR0H=0b00001011;    //setting it to first half (MSBs)
        TMR0L=0b11011011;    //setting it to second half (LSBs)
        //this will happen for every overflow
        hour = hour + 1;    //incrementing hour
        PIR0bits.TMR0IF=0; 	//clear the interrupt flag!  
    }
}