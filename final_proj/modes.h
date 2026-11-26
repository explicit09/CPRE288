#pragma once
#include "functions.h"
#include "open_interface.h"
#include "adc.h"
#include "uart.h"

extern object_data detected_objects;
extern float ir_distance_cm[91];
extern float ping_distance_cm[91];
extern int clicked;
extern int clockwise;
extern int smallest_object_index;
extern int degrees_to_turn;

void autonomous(oi_t *sensor_data);
void manual_drive(oi_t *sensor_data,int letter);
void scan_360(oi_t *sensor_data);
void auto_move(oi_t *sensor_data);
