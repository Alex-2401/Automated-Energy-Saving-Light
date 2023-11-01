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

void main(void) {
    // Initialising everything
    LCD_Init();
    Comp1_init();
	ADC_init();

    Timer0_init();
    LEDarray_init();
    Interrupts_init();
    

    unsigned int current_ADC_val = 0;
    unsigned int old_ADC_val = 0;
    // setup pin for output (connected to LED)
    LATHbits.LATH3=0;   //set initial output state
    TRISHbits.TRISH3=0; //set TRIS value for pin (output)
    LATDbits.LATD7=0;   //set initial output state
    TRISDbits.TRISD7=0; //set TRIS value for pin (output)
    
    while (1) { 
        LCD_setline(1); //Set Line 1
        current_ADC_val = ADC_getval();
        // if statement to check if value has changed
        if (current_ADC_val != old_ADC_val) { // value only updates if a change occurs
            LCD_sendbyte(0b00000001,0); //Clear Display
            __delay_ms(2);             //delay above 2ms
            ADC2String(100,current_ADC_val);
            old_ADC_val = current_ADC_val;
        }
        __delay_ms(10);
        LEDarray_disp_bin(get16bitTMR0val());
    }
}

