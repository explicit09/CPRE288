#include "scanner.h"
#include "servo.h"
#include "ping.h"
#include "Timer.h"
#include "math.h"
#include "adc.h"

extern float ping_distance_cm[91];
extern int ir_raw[91];
extern float ir_distance_cm[91];
const int degrees_to_scan = 180;
int i = 0;
int sum = 0;
int avg = 0;

void scan_ir_ping(void)
{
    for (i = 0; i <= degrees_to_scan; i += 2)
    {
        servo_move(i);
        timer_waitMillis(50);
        ping_distance_cm[i / 2] = ping_get_distance_cm();

        sum = 0;
        for (avg = 0; avg < 3; avg++)
        {
            sum += adc_read();
        }

        ir_raw[i / 2] = sum / 3;
        ir_distance_cm[i / 2] = 50316.8052 * pow(ir_raw[i / 2], -1.06489);
    }
}
void scan_ping(void)
{
    for (i = 0; i <= degrees_to_scan; i += 2)
    {
        servo_move(i);
        timer_waitMillis(50);
        ping_distance_cm[i / 2] = ping_get_distance_cm();
    }
}

void scan_ir(void)
{
    for (i = 0; i <= degrees_to_scan; i += 2)
    {
        servo_move(i);
        timer_waitMillis(50);

        sum = 0;
        for (avg = 0; avg < 3; avg++)
        {
            sum += adc_read();
        }

        ir_raw[i / 2] = sum / 3;
        ir_distance_cm[i / 2] = 50316.8052 * pow(ir_raw[i / 2], -1.06489);
    }
}
