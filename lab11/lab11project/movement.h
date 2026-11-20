#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include "open_interface.h"

void move_forward1(oi_t *sensor, int centimeters);

 void turn_clockwise(oi_t *sensor, int degrees);

 void turn_anticlockwise(oi_t *sensor, int degrees);
 void move_backwards(oi_t *sensor, int centimeters);
 void move_forward(oi_t *sensor, int centimeters);
 void object();
 void square();
#endif
