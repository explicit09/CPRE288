#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "lcd.h"

// These two varbles have been declared
// in the file containing main
extern volatile  char uart_data;  // Your UART interupt code can place read data here
extern volatile  char flag;       // Your UART interupt can update this flag
                                  // to indicate that it has placed new data
                                  // in uart_data


void uart_init(int baud);

void uart_sendChar(char data);

char uart_receive(void);

void uart_sendStr(const char *data);

void uart_interrupt_init();

void uart_interrupt_handler();
void UART1_Handler(void);
#endif /* UART_H_ */
