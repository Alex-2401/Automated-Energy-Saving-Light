#include <xc.h>
#include <stdio.h>
#include "datetime.h"
#include "LCD.h"
#include "timers.h"
#include "LEDarray.h"

extern volatile unsigned int day = 0;      //global variable for hour

unsigned int time = 0;
unsigned int minute = 0;
unsigned int hour = 0;      //global variable for hour
unsigned int month = 0;
unsigned int year = 0;
char timeString[10];
void disp_time(void)
{
    if (LATDbits.LATD7)
    {
        if (hour > 12) {LEDarray_disp_bin(0b111111111);}//need to change to accurate midday time but good enough for now 
        else {LEDarray_disp_bin(0b000000000);}
        LATDbits.LATD7 = 0;
    }
    
    if (LATHbits.LATH3) {hour++; LATHbits.LATH3 = 0;}
    if (hour >= 24) {hour = 0; day = day + 1;}      //checks if a day has passed
    if (day >= 30) {day = 1; month = month + 1;}      //checks if a day has passed
    if (month >= 13) {month = 1; year = year + 1;}
    
    if (hour == 1) {LEDarray_control();}            //1am check
    if (hour == 5) {LEDarray_disp_bin(0b111111111);}            //5am check
    

    
    time = get16bitTMR0val();   //getting timer time
    minute = (time*60)/255;    //converting to minutes
    
    LCD_setline(2); //Set Line 2
    sprintf(timeString,"%02d %02d %02d %02d %04d",minute,hour,day,month,year);  //displaying time
    LCD_sendstring(timeString);
}

