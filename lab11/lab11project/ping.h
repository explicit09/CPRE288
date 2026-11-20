#ifndef PING_H_
#define PING_H_

#include <inc/tm4c123gh6pm.h>
#include "Timer.h"


extern volatile unsigned int rising_time; //Pulse start time: Set by ISR
extern volatile unsigned int falling_time; //Pulse end time: Set by ISR


void ping_init(void);
void send_pulse();

void TIMER3B_Handler(void);
int ping_read(void);
float getDistance(void);
int getRolloverCount(void);
#endif
