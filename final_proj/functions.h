#include "uart.h"
#include "string.h"
#include "open_interface.h"

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct{
    double distance_cm[10];    // closest distance to each object (cm)
    double width[10];   // width of each object (cm)
    int angle[10];      // direction of each object (deg)
    int object_count;            // how many objects were found
}object_data;

int find_smallest(object_data *data);
void print_to_putty(char message[]);
void print_scan(char title[], float d[]);
void print_objects(object_data *data);
void detect_objects(float distance[], float ping_distance_cm[], object_data *data);
void point_to_smallest_obj(int smallest_obj_angle);

#endif
