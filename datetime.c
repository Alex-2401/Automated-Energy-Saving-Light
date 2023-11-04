#include <xc.h>
#include <stdio.h>
#include "datetime.h"
#include "LCD.h"
#include "timers.h"
#include "LEDarray.h"

unsigned int minute = 0;
unsigned int hour = 0;
unsigned int day = 28;
unsigned int month = 2;
unsigned int year = 2024;
char timeString[10];

unsigned int month_days(unsigned int month, unsigned int year) //gets number of days in a given months for a given year
{
    unsigned int monthDays;

    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)   //check for months that have 31 days
    {
        monthDays = 31;     //assigning 31 to months with 31 days
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)     //check for months that have 30 days
    {
        monthDays = 30;     //assigning 30 to months with 30 days
    }
    else if (month == 2)    //check for february
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))    //check for leap year
        {
            monthDays = 29;     //assigning 29 days (for leap year)
        }
        else {monthDays = 28;}  //assigning 28 days
    }
    return monthDays;
}

void disp_time(void) {
    if (LATDbits.LATD7)
    {
        if (hour > 12) {LEDarray_disp_bin(0b111111111);}    //need to change to accurate midday time but good enough for now 
        else {LEDarray_disp_bin(0b000000000);}
        LATDbits.LATD7 = 0;
    }
    
    if (LATHbits.LATH3) {hour++; LATHbits.LATH3 = 0;}
    if (hour >= 24) {hour = 0; day++;}      //checks if a day has passed
    if (day > month_days(month,year)) {day = 1; month++;}      //checks if a month has passed
    if (month > 12) {month = 1; year++;}    //checks if a year has passed
    
    if (hour == 1) {LEDarray_control();}              //1am check
    if (hour == 5) {LEDarray_disp_bin(0b111111111);}  //5am check
    
    minute = (get16bitTMR0val()*60)/255;    //getting timer time and converting to minutes 
    
    LCD_setline(2); //Set Line 2
    sprintf(timeString,"%02d %02d %02d %02d %04d",minute,hour,day,month,year);  //displaying time
    LCD_sendstring(timeString);
}

void LEDarray_control(void) //if i put this function her instead of in LEDarray.c i can remove a global variable
{                           //not neat but less data usage, so probably worth it
    unsigned int temp = 0;
    if (day % 3 == 0) {temp = 0b001001001;}
    if (day % 3 == 1) {temp = 0b010010010;}
    if (day % 3 == 2) {temp = 0b100100100;}
    LEDarray_disp_bin(temp);
}
