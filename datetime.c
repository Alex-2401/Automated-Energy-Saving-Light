#include <xc.h>
#include <stdio.h>
#include "datetime.h"
#include "LCD.h"
#include "timers.h"
#include "LEDarray.h"

extern volatile unsigned int hour = 0;      //global variable for hour
extern volatile unsigned int day = 0;      //global variable for hour

unsigned int time = 0;
unsigned int minutes = 0;
char timeString[10];
void disp_time(void)
{
    if (hour == 24) {hour = 0; day = day + 1;}      //checks if a day has passed
    if (hour == 1) {LATDbits.LATD7 = 0;}            //1am check
    if (hour == 5) {LATDbits.LATD7 = 1;}            //5am check
    if (hour == 1) {LEDarray_control();}            //1am check
    if (hour == 5) {LEDarray_disp_bin(0b111111111);}            //5am check
    
    time = get16bitTMR0val();   //getting timer time
    minutes = (time*60)/255;    //converting to minutes
    
    LCD_setline(2); //Set Line 2
    sprintf(timeString,"%02d %02d %03d",minutes,hour,day);  //displaying time
    LCD_sendstring(timeString);
}

