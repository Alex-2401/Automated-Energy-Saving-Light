#include <xc.h>
#include "LEDarray.h"

//volatile unsigned int hour;
volatile unsigned int day;
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
    
    ANSELGbits.ANSELG0=0; //turn off analogue input on pin
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
void LEDarray_disp_bin(unsigned int number)
{
	//some code to turn on/off the pins connected to the LED array
    //if statements and bit masks can be used to determine if a particular pin should be on/off
    if (number & 0b000000001) LATGbits.LATG0 = 1; else LATGbits.LATG0 = 0; //checking if LSB is part of number, if so the LED is set to on, if not LED is set to off 
    if (number & 0b000000010) LATGbits.LATG1 = 1; else LATGbits.LATG1 = 0; //i believe this is more efficient than the other method
    if (number & 0b000000100) LATAbits.LATA2 = 1; else LATAbits.LATA2 = 0;
    if (number & 0b000001000) LATFbits.LATF6 = 1; else LATFbits.LATF6 = 0;
    if (number & 0b000010000) LATAbits.LATA4 = 1; else LATAbits.LATA4 = 0;
    if (number & 0b000100000) LATAbits.LATA5 = 1; else LATAbits.LATA5 = 0;
    if (number & 0b001000000) LATFbits.LATF0 = 1; else LATFbits.LATF0 = 0;                                         
    if (number & 0b010000000) LATBbits.LATB0 = 1; else LATBbits.LATB0 = 0;
    if (number & 0b100000000) LATBbits.LATB1 = 1; else LATBbits.LATB1 = 0;
	//see Readme.md for examples
}

/************************************
/ Function LEDarray_disp_dec
/ Used to display a number on the LEDs
/ where each LED is a value of 10
************************************/
void LEDarray_disp_dec(unsigned int number)
{
	unsigned int disp_val = 1;
    //some code to manipulate the variable number into the correct
    //format and store in disp_val for display on the LED array    
    disp_val = (disp_val << (number + 1)) - 1; // shifts the 1 up to appropriate level then -1 to make the lights light up

	LEDarray_disp_bin(disp_val); 	//display value on LED array
}

/************************************
/ LEDarray_disp_PPM
/ Function used to display a level on the LED array with peak hold
/ cur_val is the current level from the most recent sample, and max is the peak value for the last second
/ these input values need to calculated else where in your code
************************************/
void LEDarray_disp_PPM(unsigned int cur_val, unsigned int max)
{
	unsigned int cur_disp_val=1; //assigning base values as 0001 binary
    unsigned int max_disp_val=1;
    unsigned int disp_val;
	// some code to format the variable cur_val and max, store in disp_val for display on the LED array
    // hint: one method is to manipulate the variables separately and then combine them using the bitwise OR operator
	cur_disp_val = (cur_disp_val << (cur_val + 1)) - 1; // shifts the 1 up to appropriate level then -1 to make the lights light up
    max_disp_val = (max_disp_val << max); //shifts the 1 up to appropriate level
    disp_val = cur_disp_val + max_disp_val; // adds both together
    if (cur_disp_val >= max_disp_val) {disp_val = cur_disp_val;} // if statement to check if max value should be displayed
    // hint: one method is to manipulate the variables separately and then combine them using the bitwise OR operator
	LEDarray_disp_bin(disp_val);	//display value on LED array
}
 
void LEDarray_control(void)
{
    unsigned int temp = 0;
    if (day % 3 == 0) {temp = 0b001001001;}
    if (day % 3 == 1) {temp = 0b010010010;}
    if (day % 3 == 2) {temp = 0b100100100;}
    LEDarray_disp_bin(temp);
}
