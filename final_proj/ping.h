#ifndef PING_H_
#define PING_H_

#include <inc/tm4c123gh6pm.h>

void ping_init(void);
void send_pulse();
void TIMER3B_Handler(void);
int ping_read(void);
float ping_get_distance_cm(void);
int ping_get_rollover_count(void);

#endif
