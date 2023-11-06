#include <xc.h>
#include "LEDarray.h"

/************************************
/ LEDarray_init
/ Function used to initialise pins to drive the LEDarray
************************************/
void LEDarray_init(void) // note that RC6 is not used as the resistor does not reach all the way, so the 1st LED is always off
{
    TRISGbits.TRISG0 = 0;	//set up TRIS registers for pins connected to LED array
	TRISGbits.TRISG1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISFbits.TRISF6 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 0;
    TRISFbits.TRISF0 = 0;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    
    LATGbits.LATG0 = 0;     //set initial output LAT values (they may have random values when powered on)
    LATGbits.LATG1 = 0;
    LATAbits.LATA2 = 0;
    LATFbits.LATF6 = 0;
    LATAbits.LATA4 = 0;
    LATAbits.LATA5 = 0;
    LATFbits.LATF0 = 0;
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    
    ANSELGbits.ANSELG0=0;   //turn off analogue input on pin
    ANSELGbits.ANSELG1=0;
    ANSELAbits.ANSELA2=0;
    ANSELFbits.ANSELF6=0;
    ANSELAbits.ANSELA4=0;
    ANSELAbits.ANSELA5=0;
    ANSELFbits.ANSELF0=0;
    ANSELBbits.ANSELB0=0;
    ANSELBbits.ANSELB1=0;
}

/************************************
/ LEDarray_disp_bin
/ Function used to display a number on the LED array in binary
************************************/
void LEDarray_disp_bin(unsigned int number) //this can be used to represent the street light (that function is included elsew)
{                                           
	//some code to turn on/off the pins connected to the LED array
    //if statements and bit masks can be used to determine if a particular pin should be on/off
    if (number & 0b000000001) LATGbits.LATG0 = 1; else LATGbits.LATG0 = 0; //checking if LSB is part of number, if so the LED is set to on, if not LED is set to off 
    if (number & 0b000000010) LATGbits.LATG1 = 1; else LATGbits.LATG1 = 0; 
    if (number & 0b000000100) LATAbits.LATA2 = 1; else LATAbits.LATA2 = 0;
    if (number & 0b000001000) LATFbits.LATF6 = 1; else LATFbits.LATF6 = 0;
    if (number & 0b000010000) LATAbits.LATA4 = 1; else LATAbits.LATA4 = 0;
    if (number & 0b000100000) LATAbits.LATA5 = 1; else LATAbits.LATA5 = 0;
    if (number & 0b001000000) LATFbits.LATF0 = 1; else LATFbits.LATF0 = 0;                                         
    if (number & 0b010000000) LATBbits.LATB0 = 1; else LATBbits.LATB0 = 0;
    if (number & 0b100000000) LATBbits.LATB1 = 1; else LATBbits.LATB1 = 0;
}

/************************************
/ LEDarray_control
/ function for energy saving LED lights
/ turns on 3 different street lights out of 9 every night because of even wear
************************************/
void LEDarray_control(unsigned int day) 
{                                       
    unsigned int temp = 0;
    if (day % 3 == 0) {temp = 0b001001001;}
    if (day % 3 == 1) {temp = 0b010010010;}
    if (day % 3 == 2) {temp = 0b100100100;}
    LEDarray_disp_bin(temp);
}
