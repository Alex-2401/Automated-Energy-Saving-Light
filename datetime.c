#include <xc.h>
#include <stdio.h>
#include <math.h>
#include "datetime.h"
#include "LCD.h"
#include "timers.h"
#include "LEDarray.h"

unsigned int minute = 0;
unsigned int hour = 16;
unsigned int day = 30;
unsigned int weekday = 6; //Monday = 1, Tuesday = 2, Wednesday = 3, Thurday = 4, Friday = 5, Saturday = 6, Sunday = 7
unsigned int month = 3;
unsigned int year = 2024;
unsigned int sunrisetime=0;
unsigned int sunsettime=0;
char timeString[10];
char lightString[10];
unsigned int calculatedmidday_minute = 0;
unsigned int calculatedmidday_hour = 12;
unsigned int difference = 0;
unsigned int solarnoonminutes= 0;
unsigned int eot = 0;
unsigned int longitude = 0;
unsigned int longitudestz = 0;

void Callibrate(void) {
    if (LATDbits.LATD7)
    {
        if (hour < 12) {sunrisetime = hour*60 + minute;}    // measure sunrise time
        if (hour > 12) {sunsettime = hour*60 + minute; // reset time at sunset
            
            eot = 9.87 * sin(2 * ((360 * (day - 81) / 365.0) * M_PI / 180)) -
            7.67 * sin((360 * (day - 81) / 365.0 + 78.7) * M_PI / 180); // use equation of time to find midday at times across the year
            solarnoonminutes = 12*60 + 4*(longitude - longitudestz) - eot; // solar noon in minutes
            difference = (sunsettime- sunrisetime)/2;  // difference between sunrise and midday in minutes 
            hour =(solarnoonminutes + difference)/60;    // reset hour according to midday time 
            minute = (solarnoonminutes + difference)%60;
        }
        
        LCD_setline(1); //Set Line 1
        sprintf(lightString,"%02d %02d",hour,minute);
        LCD_sendstring(lightString);
    }
}

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
    
    if (LATDbits.LATD4) {hour++; LATDbits.LATD4 = 0;}
    if (hour > 23) {hour = 0; day++; weekday++;}      //checks if a day has passed
    if (weekday > 7) {weekday = 1;}     //checks if week has passed
    if (day > month_days(month,year)) {day = 1; month++;}      //checks if a month has passed
    if (month > 12) {month = 1; year++;}    //checks if a year has passed
    
    if (hour == 1) {LEDarray_control();}              //1am check
    if (hour == 5) {LEDarray_disp_bin(0b111111111);}  //5am check
    
    if ((month == 3 || month == 10) && weekday == 7 && day > 24) {hour = DST_adjust(month,hour);}
    
    minute = (get16bitTMR0val()*60)/255;    //getting timer time and converting to minutes 
    
    LCD_setline(2); //Set Line 2
    sprintf(timeString,"%02d %02d %02d %02d %04d",minute,hour,day,month,year);  //displaying time
    LCD_sendstring(timeString);
}

unsigned int DST_adjust(unsigned int month,unsigned int hour)
{
    if (month == 3 && hour == 1 && LATHbits.LATH3 == 0)
    {
        hour = 2;
        
        LATHbits.LATH3 = 1;
    }
    if (month == 10 && hour == 2 && LATHbits.LATH3 == 1)
    {
        hour = 1;
        
        LATHbits.LATH3 = 0;
    }
    return hour;
}

void LEDarray_control(void) //if i put this function her instead of in LEDarray.c i can remove a global variable
{                           //not neat but less data usage, so probably worth it
    unsigned int temp = 0;
    if (day % 3 == 0) {temp = 0b001001001;}
    if (day % 3 == 1) {temp = 0b010010010;}
    if (day % 3 == 2) {temp = 0b100100100;}
    LEDarray_disp_bin(temp);
}
