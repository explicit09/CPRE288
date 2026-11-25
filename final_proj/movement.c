#include "movement.h"
#include "open_interface.h"
#include "stdbool.h"

const double SPEED = 100.0;

bool bumped(oi_t *sensor)
{
    return sensor->bumpLeft || sensor->bumpRight;
}

void move_forward(oi_t *sensor, double distance)
{
    double sum = 0.0;
    oi_setWheels(SPEED, SPEED); // move forward; at SPEED mm/s

    while (!bumped(sensor) && (sum < distance))
    {
        oi_update(sensor);
        sum += sensor->distance;
    }

    oi_setWheels(0, 0); // stop
}

void move_backward(oi_t *sensor, double distance)
{
    double sum = 0.0;
    oi_setWheels(-SPEED, -SPEED); // move BACKWARD; at SPEED mm/s

    while ((sum > distance)) 
    {
        oi_update(sensor);
        sum += sensor->distance;
    }

    oi_setWheels(0, 0); // stop
}

void turn_clockwise(oi_t *sensor, int degrees)
{
    double deg = 0;
    oi_setWheels(-SPEED + 20, SPEED); // -80, 100

    while (deg < degrees) 
    {
        oi_update(sensor);
        deg += fabs(sensor->angle);
    }
    oi_setWheels(0, 0); // stop
}

void turn_anticlockwise(oi_t *sensor, int degrees)
{
    double deg = 0;
    oi_setWheels(SPEED - 20, -SPEED); // 80, -100

    while (deg < degrees) 
    {
        oi_update(sensor);
        deg += fabs(sensor->angle);
    }
    oi_setWheels(0, 0); // stop
}

//correction function will need to be corrected to match final ground logic
void correction(oi_t *sensor, int turn_distance)
{
    if (sensor->bumpLeft)
    {
        move_backward(sensor, -150);
        turn_clockwise(sensor, 90);
        move_forward(sensor, turn_distance);
        turn_anticlockwise(sensor, 90);
    }
    else
    {
        move_backward(sensor, -150);
        turn_anticlockwise(sensor, 90);
        move_forward(sensor, turn_distance);
        turn_clockwise(sensor, 90);
    }
}
