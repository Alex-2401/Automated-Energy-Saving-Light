// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include <stdio.h>
#include "LCD.h"
#include "timers.h"
#include "LEDarray.h"
#include "interrupts.h"
#include "comparator.h"
#include "ADC.h"
#include "stdio.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

extern volatile unsigned int hour;      //global variable for hour

void main(void) {
    // Initialising everything
    LCD_Init();
    Comp1_init();
	ADC_init();
    Timer0_init();
    LEDarray_init();
    Interrupts_init();
    
    // needed variables
    unsigned int time = 0;
    unsigned int day = 0;
    char timeString[10];
    char lightString[4];
    // setup pin for output (connected to LED)
    LATHbits.LATH3=0;   //set initial output state
    TRISHbits.TRISH3=0; //set TRIS value for pin (output)
    LATDbits.LATD7=0;   //set initial output state
    TRISDbits.TRISD7=0; //set TRIS value for pin (output)
    
    while (1) { 

        if (hour == 24) {hour = 0; day = day + 1;}      //checks if a day has passed
        if (hour == 1) {LATDbits.LATD7 = 0;}            //1am check
        if (hour == 5) {LATDbits.LATD7 = 1;}            //5am check
        
        time = get16bitTMR0val();
        LEDarray_disp_bin(time);
        
        LCD_sendbyte(0b00000001,0); //Clear Display
        __delay_ms(2);              //delay above 2ms

        LCD_setline(1); //Set Line 1
        sprintf(lightString,"%03d",ADC_getval());
        LCD_sendstring(lightString);
        
        LCD_setline(2); //Set Line 2
        sprintf(timeString,"%03d %02d %03d",time,hour,day);
        LCD_sendstring(timeString);
        
        __delay_ms(10);
    }
}

