#include "modes.h"
#include "movement.h"
#include "scanner.h"
#include "clean.h"
#include "math.h"
#include "open_interface.h"
#include "adc.h"
#include "uart.h"

const int GO_MM = 15;
int clicked = 0;
int degrees_to_turn = 0;
int clockwise = 0;
int second_scan_smallest_index = 0;
int store_small_obect = 0;
char title[50];

void autonomous(oi_t *sensor_data)
{

    scan_ir_ping();
    //print_scan("** Raw data **\n", ir_distance_cm);
    outlier_removal(ir_distance_cm);
    moving_average(ir_distance_cm);
    detect_objects(ir_distance_cm, ping_distance_cm, &detected_objects);
    print_scan("", ping_distance_cm);
//sprintf(title, "END");
    //      print_to_putty(title);
    //print_objects(&detected_objects);

}

void auto_move(oi_t *sensor_data)
{
    if (sensor_data->bumpLeft || sensor_data->bumpRight)
    {
        print_to_putty("ohh ohh I bumped!!!!!!!!!!!!!!!");
        correction(sensor_data, 150);
        clicked -= 2;
    }

    smallest_object_index = find_smallest(&detected_objects);
    point_to_smallest_obj(detected_objects.angle[smallest_object_index]);
    clockwise = (detected_objects.angle[smallest_object_index] < 90);
    degrees_to_turn = 85 - detected_objects.angle[smallest_object_index];
    turn(sensor_data, fabs(degrees_to_turn), clockwise);
    drive_mm(sensor_data, (detected_objects.distance_cm[smallest_object_index] - 12) * 10);
    uart_sendChar('f');
    uart_sendChar('\n');
}

void manual_drive(oi_t *sensor_data, int letter)
{
    if (sensor_data->bumpLeft || sensor_data->bumpRight)
    {
        print_to_putty("ohh ohh I bumped!!!!!!!!!!!!!!!");
        correction(sensor_data, 150);
    }

    if (letter == 'w')
    {
        drive_mm(sensor_data, GO_MM);
    }
    else if (letter == 's')
    {
        drive_mm(sensor_data, -GO_MM);
    }
    else if (letter == 'a')
    {
        turn(sensor_data, 90, 0);
    }
    else if (letter == 'd')
    {
        turn(sensor_data, 90, 1);
    }
    else if (letter == 'c')
    {
        clockwise = (detected_objects.angle[smallest_object_index] < 90);
        degrees_to_turn = 90 - detected_objects.angle[smallest_object_index];
        turn(sensor_data, fabs(degrees_to_turn), clockwise);
    }
    else if (letter == 'k')
    {
        correction(sensor_data, 150); // might actually be redundant since we have the auto correction at the start of the function
    }
    else
    {
        oi_setWheels(0, 0);
    }
}

void scan_360(oi_t *sensor_data)
{
    scan_ir_ping();
    outlier_removal(ir_distance_cm);
    moving_average(ir_distance_cm);
    detect_objects(ir_distance_cm, ping_distance_cm, &detected_objects);
    smallest_object_index = find_smallest(&detected_objects);
    turn(sensor_data, 180, 1);
    scan_ir_ping();
    outlier_removal(ir_distance_cm);
    moving_average(ir_distance_cm);
    detect_objects(ir_distance_cm, ping_distance_cm, &detected_objects);
    second_scan_smallest_index = find_smallest(&detected_objects);

    /* the function has the following issues:
     * 1. detected_objects is updated during the second scan and at the moment we don't store more than
     * 91 values since all scanning functions start at int i = 0; which is the index position
     * for the array to be updated
     *
     * 2. comparing detected_objects.angle[smallest_object_index] < detected_objects.angle[second_scan_smallest_index] is invalid, as mentioned above
     * immp has been updated already and has on,y the new objects.
     *
     * 3. Our point has to somehow understand that its more than 180 degrees and it might have
     * to turn back to point the smallest object i.e. you scan the side with the smaller object
     * first and then turn, you will hae to make the roomba turns back to the side which was
     * scanned first.
     */

    if (detected_objects.angle[smallest_object_index] < detected_objects.angle[second_scan_smallest_index])
    {
        point_to_smallest_obj(smallest_object_index);
    }
    else
    {
        point_to_smallest_obj(second_scan_smallest_index);
    }
}
