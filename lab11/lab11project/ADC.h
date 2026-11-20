#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "lcd.h"

void ADCinit(void);
int ADC_read(void);

#endif
