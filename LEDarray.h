#ifndef _LEDarray_H
#define _LEDarray_H

#include <xc.h>

#define _XTAL_FREQ 64000000

//function prototypes
void LEDarray_init(void);
void LEDarray_disp_bin(unsigned int number);
void LEDarray_control(unsigned int day);

#endif
