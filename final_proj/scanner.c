#include "scanner.h"
#include "servo.h"
#include "ping.h"
#include "Timer.h"
#include "math.h"
#include "adc.h"

extern float dist[91];
extern int ir[91];
extern float estimate[91];
const int degrees_to_scan = 180;
int i = 0;
int sum = 0;
int avg = 0;

void scan_IR_PING(void)
{
    for (i = 0; i <= degrees_to_scan; i += 2)
    {
        servo_move(i);
        timer_waitMillis(50);
        dist[i / 2] = getDistance();

        sum = 0;
        for (avg = 0; avg < 3; avg++)
        {
            sum += adc_read();
        }

        ir[i / 2] = sum / 3;
        estimate[i / 2] = 50316.8052 * pow(ir[i / 2], -1.06489);
    }
}
void scan_PING(void)
{
    for (i = 0; i <= degrees_to_scan; i += 2)
    {
        servo_move(i);
        timer_waitMillis(50);
        dist[i / 2] = getDistance();
    }
}

void scan_IR(void)
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

        ir[i / 2] = sum / 3;
        estimate[i / 2] = 50316.8052 * pow(ir[i / 2], -1.06489);
    }
}
