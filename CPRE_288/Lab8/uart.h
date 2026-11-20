#ifndef UART_H_
#define UART_H_

#include "Timer.h"
#include <inc/tm4c123gh6pm.h>

extern volatile char c;
extern volatile int flag;

void uart_init(void);

void uart_sendChar(char data);

char uart_receive(void);

void uart_sendStr(const char *data);

void UART1_Handler(void);

void uart_interrupt(void);


#endif /* UART_H_ */
