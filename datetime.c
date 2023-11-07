#include <xc.h>
#include <stdio.h>
#include <math.h>
#include "datetime.h"
#include "LCD.h"
#include "timers.h"
#include "LEDarray.h"
#include "interrupts.h"
//THESE NEED TO BE SET UPON LOADUP
unsigned int second = 30;
unsigned int minute = 55;
unsigned int hour = 17;
unsigned int day = 10;
unsigned int weekday = 6; //Monday = 1, Tuesday = 2, Wednesday = 3, Thurday = 4, Friday = 5, Saturday = 6, Sunday = 7
unsigned int month = 3;
unsigned int year = 2024;
unsigned int longitude = 0;     //where you are located (England is approximatly 0)
unsigned int longitudestz = 0;  //where the time zone is located

//THESE CAN BE SET UPON LOADUP
unsigned int sunrisetime=(6*60+26);
unsigned int sunsettime=0;

//THESE ARE FOR THE PROGRAM
char timeString[10];
char syncString[10];
unsigned int difference = 0;
unsigned int solarnoonminutes= 0;
unsigned int EoT = 0;

/************************************
 * Function to initialise pins/LEDs used in datetime file
************************************/
void datetime_init(void)
{
    // These are set by the user
    TESTING_MODE = 1;//THIS IS FOR TESTING MODE (IF TESTING MODE IS ON, ITS A 1)
    TRISHbits.TRISH0=0; //set TRIS value for pin (output)
    DST_FLAG = 0;//THIS IS FOR THE DST (IF DST IS HAPPENING THEN ITS A 1)
    TRISHbits.TRISH3=0; //set TRIS value for pin (output)
}

/************************************
 * Callibration function to keep the program intime with the sun automatically
 * Used Equation of Time to modal the cyclical movement of solar midday
************************************/
void Callibrate(void) {
    if (LDR_FLAG)
    {
        if (hour < 12) {sunrisetime = hour*60 + minute;}    // measure sunrise time
        if (hour > 12) {sunsettime = hour*60 + minute; // reset time at sunset
            
            EoT = 9.87 * sin(2 * ((360 * (day - 81) / 365.0) * M_PI / 180)) - 7.67 * sin((360 * (day - 81) / 365.0 + 78.7) * M_PI / 180); // use equation of time to find midday at times across the year
            if (TESTING_MODE) {solarnoonminutes = 12*60 + 4*(longitude - longitudestz) - EoT + 1;} // solar noon in minutes
            if (!TESTING_MODE) {solarnoonminutes = 12*60 + 4*(longitude - longitudestz) - EoT;} // solar noon in minutes            
            difference = (sunsettime- sunrisetime)/2;  // difference between sunrise and midday in minutes 
            hour =(solarnoonminutes + difference)/60;    // reset hour according to midday time 
            minute = (solarnoonminutes + difference)%60;
            second = ((solarnoonminutes + difference)%60)%60;
        }       
//        LCD_setline(2); //Set Line 2
//        sprintf(syncString,"%02d:%02d:%02d,%02d",hour,minute,second,difference);
//        LCD_sendstring(syncString);
    }
}

/************************************
 * Function to find the days that each month has
 * Also checks for leap years to add a day to febuary
************************************/
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
    else if (month == 2)    //check for February
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))    //check for leap year
        {
            monthDays = 29;     //assigning 29 days (for leap year)
        }
        else {monthDays = 28;}  //assigning 28 days
    }
    return monthDays;
}

/************************************
 * Function to calculate the time
************************************/
void calc_time(void)
{
    if (TIMER_FLAG)     //checks if timer has overflown
    {
        //if statements to check if test mode or normal mode
        if (TESTING_MODE) {hour++;}     
        if (!TESTING_MODE) {second++;
            if (second > 59) {second = 0; minute++;
                if (minute > 59) {minute = 0; hour++;}
            }
        }
        TIMER_FLAG = 0; //reseting flag
    }
    
    //incrementing/calculating time
    if (TESTING_MODE) {minute = (get16bitTMR0val()*60)/255;}  //getting timer time and converting to minutes 
    if (hour > 23) {hour = 0; day++; weekday++;}      //checks if a day has passed
    if (weekday > 7) {weekday = 1;}     //checks if week has passed
    if (day > month_days(month,year)) {day = 1; month++;}      //checks if a month has passed
    if (month > 12) {month = 1; year++;}    //checks if a year has passed
    
    if ((month == 3 || month == 10) && weekday == 7 && day > 24) {hour = DST_adjust(month,hour);}   //prelimanary check for DST
}

/************************************
 * Function to display the time
************************************/
void disp_time(void)
{
    LCD_setline(1); //Set Line 1
    sprintf(timeString,"%02d:%02d %02d/%02d/%04d",hour,minute,day,month,year);  //displaying date and time
    LCD_sendstring(timeString);
}

/************************************
 * Function to activate LED lights
************************************/
void LED_activation(void)
{
    if (hour == 1) {LEDarray_control(day);}              //1am check
    if (hour == 5) {LEDarray_disp_bin(0b111111111);}  //5am check
    
    //Checks if LDR has been triggered and does appropriate lighting
    if (LDR_FLAG)
    {
        if (hour <= 5) {LEDarray_disp_bin(0b000000000);} //before 5 doesnt turn the flag off so the it keeps the lights off so they dont turn on again
        else if (hour < 12) {LEDarray_disp_bin(0b000000000); LDR_FLAG = 0;}
        if (hour > 12) {LEDarray_disp_bin(0b111111111); LDR_FLAG = 0;}
    }
}

/************************************
 * function for adjusting for daylight savings
************************************/
unsigned int DST_adjust(unsigned int month,unsigned int hour)
{
    //march check       1am check       DST off check
    if (month == 3 && hour == 1 && DST_FLAG == 0)
    {
        hour = 2;
        DST_FLAG = 1;
    }
    //october check     2am check       DST on check
    if (month == 10 && hour == 2 && DST_FLAG == 1)
    {
        hour = 1;    
        DST_FLAG = 0;
    }
    return hour;
}
