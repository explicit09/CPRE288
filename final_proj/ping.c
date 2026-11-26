#include <inc/tm4c123gh6pm.h>
#include "Timer.h"

volatile enum
{
    LOW,
    HIGH,
    DONE
} state = LOW;                      // set by ISR

volatile unsigned int rising_time;  // Pulse start time: Set by ISR
volatile unsigned int falling_time; // Pulse end time: Set by ISR
volatile unsigned int rollover_count;

void TIMER3B_Handler(void)
{
    if (TIMER3_MIS_R & 0x400)
    {
        TIMER3_ICR_R = 0x400;
        unsigned int timestamp = TIMER3_TBR_R & 0xFFFFFF;

        if (state == LOW)
        {
            rising_time = timestamp;
            state = HIGH;
        }
        else if (state == HIGH)
        {
            falling_time = timestamp;
            state = DONE;
        }
    }
}

void ping_init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x02;
    SYSCTL_RCGCTIMER_R |= 0x08;

    timer_waitMicros(10);

    GPIO_PORTB_DEN_R |= 0x08;
    GPIO_PORTB_DIR_R |= 0x08;
    GPIO_PORTB_AFSEL_R &= ~0x08;
    GPIO_PORTB_PCTL_R &= 0xFFFF0FFF; // changing B wire 3 to alternate function
    GPIO_PORTB_PCTL_R |= 0x00007000; // program alt function

    TIMER3_CTL_R &= ~0x100;
    TIMER3_CFG_R = 0x04;
    TIMER3_TBMR_R = 0x07;
    TIMER3_TBILR_R = 0xFFFF;
    TIMER3_TBPR_R = 0xFF;
    TIMER3_CTL_R |= 0xC00;
    TIMER3_IMR_R &= ~0x400;
    TIMER3_ICR_R = 0x400;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    NVIC_EN1_R |= 0x10;
    IntMasterEnable();
    TIMER3_CTL_R |= 0x100;
}

void send_pulse()
{
    TIMER3_IMR_R &= ~0x400;

    GPIO_PORTB_AFSEL_R &= ~0x08;
    GPIO_PORTB_DIR_R |= 0x08;
    GPIO_PORTB_DATA_R &= ~0x08;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R |= 0x08; //high
    timer_waitMicros(5);

    GPIO_PORTB_DATA_R &= ~0x08; //low
    GPIO_PORTB_DIR_R &= ~0x08;
    GPIO_PORTB_AFSEL_R |= 0x08;

    // Clear flag and enable capture interrupt
    TIMER3_ICR_R = 0x400;
    TIMER3_IMR_R |= 0x400;
}

int ping_read(void) // Get distance from PING sensor
{
    int clock_cycles;
    // Wait for ISR to capture rising edge & falling edge time
    // Calculate the width of the pulse; convert to centimeters
    state = LOW;
    // Trigger the ping sensor
    send_pulse();
    while (state != DONE){}

    TIMER3_IMR_R &= ~0x400;
    clock_cycles = (rising_time - falling_time) & 0xFFFFFF;
    if (falling_time > rising_time)
    {
        rollover_count++;
    }
    return clock_cycles;
}

// send out a pulse on PB3
// missing details!!, see lab�s timing diagram
float ping_get_distance_cm(void)
{
    int clock_cycles;
    float distance;
    clock_cycles = ping_read();
    distance = (clock_cycles * 34000.0f) / (2 * 16000000);
    return distance;
}

int ping_get_rollover_count(void)
{
    return rollover_count;
}
