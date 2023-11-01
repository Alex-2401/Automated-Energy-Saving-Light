// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "LCD.h"
#include "timers.h"
#include "LEDarray.h"
#include "LCD.h"
#include "interrupts.h"
#include "comparator.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    // Initialising everything
    LCD_Init();
    Timer0_init();
    LEDarray_init();
    LCD_Init();
    Interrupts_init();
    Comp1_init();
    
    while (1) { 
        LCD_scroll(); // scrolling through to make car move
        __delay_ms(400); //delaying so it is visible
    }
}

