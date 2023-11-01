#include <xc.h>
#include "LCD.h"
#include "stdio.h"

/************************************
 * Function to toggle LCD enable bit on then off
 * when this function is called the LCD screen reads the data lines
************************************/
void LCD_E_TOG(void)
{
	//turn the LCD enable bit on    
    LCD_E = 1;
	__delay_us(5); //wait a short delay
	//turn the LCD enable bit off again
    LCD_E = 0;
}

/************************************
 * Function to set the 4-bit data line levels for the LCD
************************************/
void LCD_sendnibble(unsigned char number)
{
	//set the data lines here (think back to LED array output)
    if (number & 0b000000001) LCD_DB4 = 1; else LCD_DB4 = 0;    
    if (number & 0b000000010) LCD_DB5 = 1; else LCD_DB5 = 0;
    if (number & 0b000000100) LCD_DB6 = 1; else LCD_DB6 = 0;
    if (number & 0b000001000) LCD_DB7 = 1; else LCD_DB7 = 0;
    LCD_E_TOG();			//toggle the enable bit to instruct the LCD to read the data lines
    __delay_us(50);      //Delay 5uS
}

/************************************
 * Function to send full 8-bit commands/data over the 4-bit interface
 * high nibble (4 most significant bits) are sent first, then low nibble sent
************************************/
void LCD_sendbyte(unsigned char Byte, char type)
{
    // set RS pin whether it is a Command (0) or Data/Char (1) using type argument
    LCD_RS = type;
    // send high bits of Byte using LCDout function
    LCD_sendnibble(Byte >> 4);
    __delay_us(50);      
    // send low bits of Byte using LCDout function
	LCD_sendnibble(Byte);
    __delay_us(50);               //delay 50uS (minimum for command to execute)
}

/************************************
 * Function to initialise the LCD after power on
************************************/
void LCD_Init(void)
{
    //Define LCD Pins as Outputs and
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE1 = 0;

    //set all pins low (might be random values on start up, fixes lots of issues)
    LCD_RS = 0;     //C6 pin
    LCD_E = 0;      //C2 pin
    LCD_DB4 = 0;    //B3 pin
    LCD_DB5 = 0;    //B2 pin
    LCD_DB6 = 0;    //E3 pin
    LCD_DB7 = 0;    //E1 pin
    //Initialisation sequence code
	// follow the sequence in the GitHub Readme picture for 4-bit interface.
	// first Function set should be sent with LCD_sendnibble (the LCD is in 8 bit mode at start up)
    //Special case of 'Function Set' over and over to initialise the LCD
    __delay_ms(150);            //delay more than 100ms
    LCD_sendnibble(0b0011);
    __delay_ms(5);              //delay more than 4.1ms
    LCD_sendnibble(0b0011);
    __delay_us(200);            //delay more than 100us
    LCD_sendnibble(0b0011);
    __delay_us(200);            //delay more than 100us
    LCD_sendnibble(0b0010);     //setting to 4-bit mode
    __delay_us(200);            //delay more than 100us
	// after this use LCD_sendbyte to operate in 4 bit mode
    LCD_sendbyte(0b00101100,0); //N and F set to 1
    __delay_us(100);            //delay more than 53us
    LCD_sendbyte(0b00001000,0); //Display ON/OFF Control set to 1, rest set to 0
    __delay_us(100);            //delay more than 53us
    LCD_sendbyte(0b00000001,0); //Clear Display
    __delay_ms(5);              //delay more than 3ms
    LCD_sendbyte(0b00000110,0); //Entry mode set I/D = 1, S = 0
    __delay_us(100);            //delay more than 53us
    //Initialization ends    
	//remember to turn the LCD display back on at the end of the initialisation (not in the data sheet)
    LCD_sendbyte(0b00001100,0); //Display ON/OFF Control set to 1, D set to 1, rest set to 0
    __delay_us(100);            //delay more than 53us
}

/************************************
 * Function to set the cursor to beginning of line 1 or 2
************************************/
void LCD_setline (char line)
{
    //Send 0x80 to set line to 1 (0x00 ddram address)
    if (line == 1)
        LCD_sendbyte(0x80,0);
    //Send 0xC0 to set line to 2 (0x40 ddram address)
    if (line == 2)
        LCD_sendbyte(0xC0,0);
}

/************************************
 * Function to send string to LCD screen
************************************/
void LCD_sendstring(char *string)
{	  
	while(*string != 0){  // While the data pointed to isn?t a 0x00 do below (strings in C must end with a NULL byte) 
		LCD_sendbyte(*string++,1); 	//Send out the current byte pointed to and increment the pointer
	}
}

/************************************
 * Function to send string to LCD screen
************************************/
void LCD_scroll(void)
{
	//code here to scroll the text on the LCD screen
    LCD_sendbyte(0b00011000,0); //Scroll 1 position to the left
    __delay_us(50); //delay above 39us
}

/************************************
 * Function takes a ADC value and works out the voltage to 2 dp
 * the result is stored in buf as ascii text ready for display on LCD
 * Note result is stored in a buffer using pointers, it is not sent to the LCD
************************************/
void ADC2String(char *buf, unsigned int ADC_val){
	//code to calculate the integer and fractions part of a ADC value
    unsigned int int_part = ADC_val/77; //255/3.3 = 77
    unsigned int frac_part = (ADC_val*100)/77 - int_part *100;
	// and format as a string using sprintf (see GitHub readme)
    sprintf(buf,"%d.%02d",int_part,frac_part);
    LCD_sendstring(buf);
}