#ifndef _datetime_H
#define _datetime_H

#include <xc.h>

#define _XTAL_FREQ 64000000

//Define Pins/LEDs Used as flags
#define TESTING_MODE LATHbits.LATH0
#define DST_FLAG LATHbits.LATH3
  
//function prototypes
void datetime_init(void);
void calc_time(void);
void disp_time(void);
void LED_activation(void);
unsigned int month_days(unsigned int month,unsigned int year);
unsigned int DST_adjust(unsigned int month,unsigned int hour);
void Callibrate(void);

#endif