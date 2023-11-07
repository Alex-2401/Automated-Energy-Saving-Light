#include <xc.h>
#include "interrupts.h"
#include "LEDarray.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
 * Also initialises pins/LEDs needed for interrupts
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
    
    // These are pins for the program 
    TRISDbits.TRISD7=0; //set TRIS value for pin (output)
    LDR_FLAG = 0;   //FLAG FOR THE LDR "sunset", "sunrise"
    TRISDbits.TRISD4=0; //set TRIS value for pin (output)
    TIMER_FLAG = 0; //FLAG FOR TIMER OVERFLOW
}

/************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
 * This interrupt is for the LDR
 * It triggers on "sunset" and "sunrise"
 * Sets an LED to on (it will be turned off almost instantly)
************************************/
void __interrupt(high_priority) HighISR()
{
    //add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    if(PIR2bits.C1IF){ 	//check the interrupt source
        LDR_FLAG = 1;
        PIR2bits.C1IF=0; 	//clear the interrupt flag!       
    }
}

/************************************
 * Low priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
 * This interrupt is for the timer
 * It will trigger when overflowed
 * Resets the starting value so that it is one second
************************************/
void __interrupt(low_priority) LowISR()
{
    //add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    if(PIR0bits.TMR0IF){ 	//check the interrupt source
        TMR0H=0b00001011;    //setting it to first half (MSBs)
        TMR0L=0b11011011;    //setting it to second half (LSBs)
        //this will happen for every overflow
        TIMER_FLAG = 1;
        PIR0bits.TMR0IF=0; 	//clear the interrupt flag!  
    }
}