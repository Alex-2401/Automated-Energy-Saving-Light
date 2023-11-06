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
    datetime_init();
    
    // needed variables
    char lightString[4];
    
    while (1)
    { 
        
//        LCD_setline(1); //Set Line 1
//        sprintf(lightString,"%03d",ADC_getval());
//        LCD_sendstring(lightString);
        
        Callibrate();
        calc_time();
        LED_activation();
        disp_time();
        
        __delay_ms(10);
    }
}

