/*
 * movement.h
 *
 *  Created on: Sep 3, 2026
 *      Author: kausalya
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"

void move_forward(oi_t *sensor, double distance); 
void move_backward(oi_t *sensor, double distance);
void turn_clockwise(oi_t *sensor, int degrees);
void turn_anticlockwise(oi_t *sensor, int degrees);

void correction(oi_t *sensor, int turn_distance);


#endif /* MOVEMENT_H_ */
