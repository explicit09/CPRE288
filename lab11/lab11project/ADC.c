#include "lcd.h"
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>


void ADCinit(void){
    SYSCTL_RCGCGPIO_R |= 0x02;
    timer_waitMillis(10);
    GPIO_PORTB_AFSEL_R |=  0x10;
    GPIO_PORTB_DIR_R &= 0xEF;
    GPIO_PORTB_DEN_R &=0xEF;
    GPIO_PORTB_AMSEL_R |= 0x10;
    SYSCTL_RCGCADC_R |= 0x01;
    timer_waitMillis(10);
    ADC0_ACTSS_R |= 0x02;
    ADC0_EMUX_R &= 0xFF0F;
   // ADC0_PSSI_R |= 0x02;
    //ADC0_SSMUX1_R &= 0xFFF0;
    ADC0_SSCTL1_R |= 0x06;
    ADC0_SSMUX1_R = 0x000A;
    ADC0_SAC_R = 0x4;

}
int ADC_read(void){
    ADC0_PSSI_R |= 0x02;
    ADC0_IM_R |= 0x2;
    while((~ADC0_RIS_R & 0x02)){

    }
    int x;

    x = ADC0_SSFIFO1_R;
    ADC0_ISC_R |= 0x01;
    return x;



}
