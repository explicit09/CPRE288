#include "init.h"
#include "common.h"
#include "adc.h"
#include "servo.h"
#include "ping.h"

void init(void)
{
    timer_init();
    lcd_init();
    uart_init();
    uart_interrupt();

    adc_init();
    servo_init();
    ping_init();
}