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
#include "datetime.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    // Initialising everything
    LCD_Init();
    Comp1_init();
	ADC_init();
    Timer0_init();
    LEDarray_init();
    Interrupts_init();
    
    // needed variables
    char lightString[4];
    // setup pin for output (connected to LED)
    LATHbits.LATH3=0;   //THIS IS FOR THE DST (IF DST IS HAPPENING THEN ITS A 1)
    TRISHbits.TRISH3=0; //set TRIS value for pin (output)
    LATHbits.LATH0=1;   //THIS IS FOR TESTING MODE (IF TESTING MODE IS ON, ITS A 1)
    TRISHbits.TRISH0=0; //set TRIS value for pin (output)
    
    LATDbits.LATD7=0;   //set initial output state
    TRISDbits.TRISD7=0; //set TRIS value for pin (output)
    LATDbits.LATD4=0;   //set initial output state
    TRISDbits.TRISD4=0; //set TRIS value for pin (output)    
    
    while (1)
    { 
//        LCD_setline(1); //Set Line 1
//        sprintf(lightString,"%03d",ADC_getval());
//        LCD_sendstring(lightString);
        
        Callibrate();
        disp_time();
        
        __delay_ms(10);
    }
}

