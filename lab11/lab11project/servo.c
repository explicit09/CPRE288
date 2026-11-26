#include "Timer.h"
#include "servo.h"
#include <inc/tm4c123gh6pm.h>

void servo_init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x02; // enables port B
    SYSCTL_RCGCTIMER_R |= 0x02; // enables timer clock
    timer_waitMicros(10);

    TIMER1_CTL_R &= ~0x100; // disables Timer 1B

    GPIO_PORTB_DEN_R |= 0x20;
    GPIO_PORTB_DIR_R |= 0x20; // PB5 as output
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R &= 0xFF0FFFFF;
    GPIO_PORTB_PCTL_R |= 0x00700000; // selects T1CCP1

    TIMER1_CFG_R = 0x04; // 16-bit split timer
    TIMER1_TBMR_R = 0x0A; // periodic PWM mode
    TIMER1_TBPR_R = 0x04;
    TIMER1_TBILR_R = 0xEF53;
    TIMER1_TBPMR_R = 0x04;
    TIMER1_TBMATCHR_R = 0xCC00;

    TIMER1_CTL_R &= ~0x4000;
    TIMER1_CTL_R |= 0x100; // enables Timer 1B
}

void servo_move(int degrees)
{
    // Calculate match value for desired position
    TIMER1_TBMATCHR_R = 0xCC00 - (155 * degrees);
}