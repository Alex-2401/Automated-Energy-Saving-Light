#ifndef _datetime_H
#define _datetime_H

#include <xc.h>

#define _XTAL_FREQ 64000000

//function prototypes
void disp_time(void);
unsigned int month_days(unsigned int month, unsigned int year);
void LEDarray_control(void);

#endif