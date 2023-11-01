#include <xc.h>
#include "ADC.h"

/************************************
/ ADC_init_init
/ Function used to initialise ADC module and set it up
/ to sample on pin RA3
************************************/
void ADC_init(void)
{
    TRISAbits.TRISA3=1; // Select pin RA3 as input
    ANSELAbits.ANSELA3=1; //Ensure analogue circuitry is active (it is by default - watch out for this later in the course!)

    // Set up the ADC module - check section 32 of the datasheet for more details
    ADREFbits.ADNREF = 0; // Use Vss (0V) as negative reference
    ADREFbits.ADPREF = 0b00; // Use Vdd (3.3V) as positive reference
    ADPCH=0b11; // Select channel RA3/ANA3 for ADC
    ADCON0bits.ADFM = 0; // Left-justified result (i.e. no leading 0s)
    ADCON0bits.ADCS = 1; // Use internal Fast RC (FRC) oscillator as clock source for conversion
    ADCON0bits.ADON = 1; // Enable ADC
}

unsigned int ADC_getval(void)
{
    unsigned int tmpval;
    ADCON0bits.GO = 1; // Start ADC conversion

    while (ADCON0bits.GO); // Wait until conversion done (bit is cleared automatically when done)
        
    tmpval = ADRESH; // Get 8 most significant bits of the ADC result - if we wanted the 
	// full 10bit result we would also need to read the 2 LSBs from ADRESL and combine the result.
	// An 8bit result is sufficient for our needs here
    
    tmpval = 255 - tmpval; //invert readings for LDR so brighter is a bigger number

    //min value is 35
    //max value is 151
    //subject to change depending on environment
    
    tmpval = ((tmpval - 35 + 2) *255)/(151-35+2) ; // scaling from 0 to 255 (added 2 for redundancy)
    if (tmpval > 512) {tmpval = 0;} //check just incase above statement ends up negative, it will become a huge positive number (integer limit)
                                   //arbitrary number (this is 2x what the scale is) as when moving to new environment the scale will be off
    return tmpval; //return this value when the function is called
}
