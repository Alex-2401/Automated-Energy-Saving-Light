// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "LCD.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    LCD_Init();
    // making a car animation
    LCD_setline(1); //Set Line 1
    LCD_sendbyte(0b11010101,1); // drivers window
    LCD_sendbyte(0b11001001,1); // box
    LCD_sendbyte(0b10111111,1); // box        
    LCD_sendbyte(0b10111111,1); // box
    LCD_sendbyte(0b10111111,1); // box
    LCD_sendbyte(0b00011001,1); // boot
    LCD_sendbyte(0b00100000,1); // space       
    LCD_sendbyte(0b00100000,1); // space 
    LCD_sendbyte(0b00100000,1); // space 
    LCD_sendbyte(0b00101110,1); // fumes        
    LCD_sendbyte(0b10110001,1); // fumes 
    LCD_setline(2); //Set Line 2
    LCD_sendbyte(0b10111111,1); // front bumper
    LCD_sendbyte(0b01001111,1); // wheel       
    LCD_sendbyte(0b00101101,1); // car base
    LCD_sendbyte(0b00101101,1); // car base
    LCD_sendbyte(0b01001111,1); // wheel
    LCD_sendbyte(0b00111101,1); // exhaust
    LCD_sendbyte(0b00100000,1); // space         
    LCD_sendbyte(0b10110010,1); // fumes
    LCD_sendbyte(0b10110001,1); // fumes  
    while (1) { 
        LCD_scroll(); // scrolling through to make car move
        __delay_ms(400); //delaying so it is visible
    }
}

