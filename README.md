[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/lWhCsCZp)
# Mini project - Energy saving automatic outside light

## Table of Contents

1. Hardware Configureation
2. User Setup
3. Breif
4. Specification
5. Program Explanation
6. Program Architecture

## 1. Hardware Configureation

The hardware is the same setup as in lab 4, except there is an extra wire (for the LDR interrupt) connected to pin RA3

A Picture of the setup is shown below:

![image](https://github.com/ME3-HECM/mini-project-alex-dayo-ecm/assets/147276883/f8aece3c-f93a-4dd4-b598-250aae5da6cd)

The brown wire on the left is the connection from the LDR to the pin RA3

## 2. User Manual
The program needs some user input before starting so that the timing is accurate

The variables/values that are needed are the date, time and position

The date and time can be defined in 7 variables

1. second (0 to 59)
2. minute (0 to 59)
3. hour (0 to 24)
4. day (0 to 31)
5. weekday (Monday = 1, Tuesday = 2, Wednesday = 3, Thurday = 4, Friday = 5, Saturday = 6, Sunday = 7)
6. month (1 to 12)
7. year (0 to 9999)
The position can be defined in 2 variables
1. longitude (longitude where you are located)
2. longitudestz (longitude where the time zone is located)

The variables/values that are suggested to be defined are

1. sunrisetime (time of the sun rise on a given day)
2. sunsettime (time of the sun set on a given day)

this is because if they are not set, it could mess up the calibration of the clock

Some flags that will need to be set are:

- TESTING_MODE (if the program is put in testing mode, one hour will pass every second, to turn it on set it to 1)
- DST_FLAG (this is for daylight savings, if the time you set th eprogram to be at is daylight savings time, set this to 1)

## 3. Brief

Outside lights that respond to ambient light levels are commonplace (i.e. street lights, porch lights, garden lights etc). These types of lights switch on at dusk and then turn off at dawn. However, energy and money can be saved if these lights are switched off during the small hours of the morning (for example, between 1am and 5am), when there are very few people around. Many councils in the UK have implemented/trialled this idea for street lights (https://www.bbc.co.uk/news/uk-england-derbyshire-16811386). Your task is to use the knowledge of microcontrollers and hardware that you have gained in labs 1-3 from this module to develop a fully automated solution.

## 4. Specification
Design and program a device that meets the following requirements:

1. Monitors light level with the LDR and turns on an LED in low light conditions (i.e. night-time) and off in bright conditions (i.e. daytime)
1. Displays the current hour of day on the LED array in binary
1. Turns the light off between approx. 1am and 5am
1. Adjusts for daylight savings time
1. Maintain synchronicity with the sun indefinitely
1. Be fully automatic (requires zero maintenance after installation)

Please use this GitHub repo to manage your software development and submit your mini project code.

## 5. Program Explanation

In this section, I will address each of the specifications one by one to demonstrate the methods utilized to achieve them:

### 1. Monitors light level with the LDR and turns on an LED in low light conditions (i.e. night-time) and off in bright conditions (i.e. daytime):

A comparator function is employed to monitor the light levels using a Light Dependent Resistor (LDR).

This function triggers a high priority interrupt when a significant change in light is detected.

The change triggers a flag using the D7 pin, which is an efficient use of resources as it avoids the need for a global variable.

The system distinguishes between sunrise and sunset by recording the time of the light level change.

If the change happens after midday, it is identified as sunset, and the LED is turned on. Conversely, if the change occurs before midday, it is sunrise, and the LED is turned off.

### 2. Displays the current hour of day on the LED array in binary:

In the original design, the time was represented in binary on an LED array. However, this was improved by displaying the time on an LCD screen, which provides a clearer representation of the current hour, alongside the day, month, and year.

It can also display a number of other variables, which can be chosen if needed.

### 3. Turns the light off between approx. 1am and 5am:

An if function activates a power-saving mode between 1 am and 5 am.

The streetlights, represented by the LED display, are adjusted during these hours. Typically, all lights are on, but during power-saving hours, only one-third are illuminated. These lights change every night to promote even wear across the LEDs

A high priority interrupt is set for the LDR to ensure that if sunrise occurs before 5 am, all lights are turned off.

### 4. Adjusts for daylight savings time:

The system checks if the current month is March or October and if the current day is a Sunday after the 24th (31 (both months have 31 days) - 7 = 24) of the month, which aligns with the usual occurrence of daylight saving adjustments.
Within the adjustment function, the system checks the month and determines whether to move the hour forward or back based on the daylight saving trigger pin's state.

### 5. Maintain synchronicity with the sun indefinitely:

The system uses the recorded times of sunrise and sunset to calculate midday, considering the longitude to adjust for any global location.

By halving the difference between sunrise and sunset times and adding it to the calculated midday, the system recalibrates the clock at sunset, ensuring it remains in sync with the sun's movements.

Midday is calculated using the simplified form of the EoT (Equation of Time), and the longnitude mentioned earlier.

<img width="566" alt="image" src="https://github.com/ME3-HECM/mini-project-alex-dayo-ecm/assets/147276883/3269b7b2-7038-4f99-b603-9e970f8d7d1b">

<img width="767" alt="image" src="https://github.com/ME3-HECM/mini-project-alex-dayo-ecm/assets/147276883/a06f8421-096e-4427-ba87-d71829a6efd1">

More information on this topic can be found below: https://en.wikipedia.org/wiki/Equation_of_time

Here is the graph that is produced when using this equation (before and after adjusting for daylight savings):

<img width="457" alt="image" src="https://github.com/ME3-HECM/mini-project-alex-dayo-ecm/assets/147276883/1e553980-9fac-435c-9f41-b88201b6a7fc">

<img width="405" alt="image" src="https://github.com/ME3-HECM/mini-project-alex-dayo-ecm/assets/147276883/817c3562-86df-47fc-a581-cbcead90a02d">


### 6. Be fully automatic (requires zero maintenance after installation):

The entire process is automated, from light monitoring and LED control to daylight saving adjustments and synchronization with the sun.

Once installed, the system requires no maintenance, as it self-adjusts based on the input from the LDR and the internal clock aswell as the EoT function discribed earlier.

## 6. Program Architecture

### 1. main.c
- calls the initialising functions of all modules.
- Executes main continuous loop that runs the program.

### 2. timers.c/h
- Initialises Timer0 with a specific pre-scaler and post-scaler to overflow every second.
- Provides a function to retrieve the 16-bit value from Timer0.

### 3. interrupts.c/h
- Initialises interrupts, enabling global, peripheral, and specific interrupts for the timer and comparator with priority settings.
- High priority interrupt service routine (ISR) sets the LDR flag when a comparator interrupt occurs, indicating a change in light levels (sunset/ sunrise).
- Low priority ISR handles timer overflow, resetting the timer and setting a timer flag for the main program to handle.

### 4. ADC.c/h
- Initialises the ADC module to read analog values from a specific pin (RA3).
- Provides a function to get the current value from the ADC. (Useful but not used in this specific version of the code)

### 5. LCD.c/h
- Contains functions to interface with an LCD, including initialization and commands to send data and set cursor positions.
- Manages the enabling and disabling of the LCD to read data lines.
- Sends characters and strings to the LCD for display.

### 6. LEDarray.c/h
- Initialises pins connected to the LED array.
- Contains functions to display binary numbers on the LED array and control the LED array for energy-saving purposes based on the day of the month.

### 7. comparator.c/h
- Initialises Digital-to-Analog Converter (DAC) and a comparator.

### 8. datetime.c/h
- Contains variables and functions to manage the datetime functionality.
Initializes datetime settings, including testing mode and daylight saving time (DST) flag.
- Callibrate function adjusts time based on sun position using the Equation of Time.
- calc_time function manages the incrementing of time and adjusts for overflow in seconds, minutes, hours, days, and months, including leap years.
- disp_time function displays the current time on the LCD.
- LED_activation function manages the LED array based on LDR flag and time.
- DST_adjust function adjusts the hour for daylight savings time based on the month and hour.

## Learning outcomes

The principal learning objectives for this project are:

- Implement a working microprocessor based system to achieve a more complex real world task
- Develop your ability to independently plan, organise and structure your code 
- Improve your grasp of the C language and writing your own functions

## Supplementary information and help
At first the task may seem quite straightforward but there are several points that often prove more tricky. The first is how to test code during development? You could test in real world conditions but you would be limited to one test cycle per day and this would severely slow down your development and debugging progress. To get around this you could implement a "testing mode" and pretend that a day lasts 24 seconds. This could be done using a #define directive to switch between "normal" and "testing" modes for your code.

Adjusting for daylight savings time is not too tricky. The clocks always change (in the UK) on the last Sunday in March (they go forward an hour) and the last Sunday in October (they go back an hour). One method of achieving this is to initialise what day it is when device is first switched on (using manual input) and then keep track of the days that pass and what the day of the week it is. Another method might be to automatically figure out what time of year it is (see below). Also don't forget about leap years! 

No clock is perfect, they can all run slightly fast/slow and can by influenced be external factors such as temperature. Ultimately this will result in drift over time and eventually the time will drift so far out of sync with real time that it is meaningless. For the purposes of our device the main requirement is that it remains in sync with the sun. You could use light from the sun to keep your clock in sync. Although the length of daylight varies considerably during the year, the midpoint between dusk and dawn only varies by a few minutes. This is termed solar midnight approx. 12am or solar noon approx. 12pm. One method of staying in sync with the sun is to use the LDR and record/calculate when these times occur and adjust your clock accordingly. The length of daylight also tells us information about what time of year it is and can be used to help us know when to adjust for daylight savings time.

![Day length](gifs/day-length-london.jpg)
http://wordpress.mrreid.org/2010/10/31/why-change-the-clocks/





