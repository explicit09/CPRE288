#include "movement.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "uart.h"
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "open_interface.h"
void move_backwards(oi_t *sensor, int centimeters){
    oi_setMotorCalibration(1.00, 1.0);
    double sum = 0;
    oi_setWheels(-100, -100); // move forward; full speed
    while (fabs(sum) < centimeters) {
        oi_update(sensor);
        sum += sensor->distance;
    }
    oi_setWheels(0, 0); // stop
}
void move_forward1(oi_t *sensor, int centimeters){
    oi_setMotorCalibration(1.00,1.0);
    double sum = 0;
    oi_setWheels(100,100);
    while(sum < centimeters){
    oi_update(sensor);
    sum += sensor->distance;
    }
    oi_setWheels(0, 0);
}


void turn_clockwise(oi_t *sensor, int degrees){
    oi_setMotorCalibration(1.00, 1.0);
    double sum = 0;
    oi_setWheels(-50, 50);
    while (fabs(sum) <= (fabs(degrees))) {
        oi_update(sensor);
        sum += sensor->angle;
    }
    oi_setWheels(0, 0); // stop
}

void turn_anticlockwise(oi_t *sensor, int degrees){
    double sum = 0;
    oi_setWheels(50, -50);
    while (sum < (degrees)) {
        oi_update(sensor);
        sum += sensor->angle;
    }
    oi_setWheels(0, 0); // stop
}
void move_forward(oi_t *sensor, int centimeters){
    oi_setMotorCalibration(1.00, 1.0);
    double sum = 0;
    oi_setWheels(100, 100); // move forward; full speed
    while (sum < centimeters) {
        oi_update(sensor);
        if(sensor->bumpLeft){
            move_backwards(sensor,75);
            turn_clockwise(sensor,86);
            move_forward1(sensor,100);
            turn_anticlockwise(sensor,86);
            uart_sendStr("hit an object with the left bumper");
            return;
        }else if(sensor->bumpRight){
            move_backwards(sensor,75);
            turn_anticlockwise(sensor,86);
            move_forward1(sensor,100);
            turn_clockwise(sensor,86);
            uart_sendStr("hit an object with the right bumper");
            return;
        }else if(sensor->bumpLeft && sensor->bumpRight){
            move_backwards(sensor,75);
            turn_clockwise(sensor,86);
            move_forward1(sensor,100);
             turn_anticlockwise(sensor,86);
             uart_sendStr("hit an object with both bumpers");
        }
        sum += sensor->distance;
    }
    oi_setWheels(0, 0); // stop
}
void object(){
    oi_t *sensor_data = oi_alloc();
        oi_init(sensor_data);

        oi_update(sensor_data);
        // get current state of all sensors
        int i = 0;
        int x;

        while (i< 1970){
            oi_setWheels(100, 100);
                    oi_update(sensor_data);
                    i += sensor_data->distance;
                    x = i;
            if (sensor_data->bumpLeft && sensor_data->bumpRight) {
                     // both triggered - pick a direction
                     move_backwards(sensor_data, 75);
                     turn_clockwise(sensor_data, 83);
                     move_forward(sensor_data, 100);
                     turn_anticlockwise(sensor_data, 83);
                     i = x -150;//offsets the going back 15 cm
            }
                     else if (sensor_data->bumpLeft) {
            move_backwards(sensor_data, 150);
              turn_clockwise(sensor_data, 82);
              move_forward(sensor_data, 250);
              turn_anticlockwise(sensor_data, 83);
              i = x - 150;


        }
        else if (sensor_data->bumpRight) {
                          move_backwards(sensor_data, 150);
                          turn_anticlockwise(sensor_data, 83);
                            move_forward(sensor_data, 250);
                            turn_clockwise(sensor_data, 83);
                            i = x - 150;


              }






        }



        oi_setWheels(0, 0);
       oi_free(sensor_data);
       return;
}
void square() {
    oi_t *sensor_data = oi_alloc();
        oi_init(sensor_data);
        int i;
       for( i=0; i< 4; i++){
       move_forward(sensor_data, 500);
       turn_clockwise(sensor_data, 80);

       }
       oi_free(sensor_data);
}
