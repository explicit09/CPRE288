#include "functions.h"
#include "stdio.h"
#include "math.h"
#include "servo.h"

void print_to_putty(char message[]) 
{
    int k;
    for (k = 0; k < strlen(message); k++)
    {
        uart_sendChar(message[k]);
    }
}

void print_scan(char title[], float d[])
{
    char message[50];
    int k;

    for (k = 0; k <  91; k++)
    {
        sprintf(message, "%d\t\t%.1f\n", k * 2, d[k]);
        print_to_putty(message);
    }
}

void print_objects(object_data *data)
{
    char message[50];
    int k;

    print_to_putty("Object#\tAngle\tDistance\tWidth\n");
    for (k = 1; k <= data->object_count; k++)
    {
        sprintf(message, "%d\t%d\t%.1f\t\t%.1f\n", k, data->angle[k],
                data->distance_cm[k], data->width[k]);
        print_to_putty(message);
    }
}

static void add_object(float distance[], object_data *data, int first, int last)
{
    double d1 = distance[first];
    double d2 = distance[last];
    double closest = d1;
    double span = (last - first) * 2.0;
    int j, n;

    if (last - first + 1 < 2)
        return;

    for (j = first; j <= last; j++)
    {
        if (distance[j] < closest)
            closest = distance[j];
    }
    
    data->object_count++;
    n = data->object_count;
    data->angle[n] = first + last; 
    data->distance_cm[n] = closest;
    data->width[n] = sqrt(pow(d1,2) + pow(d2,2)
                          - 2.0 * d1 * d2 * cos(span * (M_PI / 180.0)));
}

// Detects object boundaries from distance changes of at least 20 cm.
void detect_objects(float distance[], float ping_distance_cm[], object_data *data)
{
    int first = -1;         // -1 means no object is being tracked.
    int idx;
    float diff;

    data->object_count = 0;
    data->angle[0] = 90;    // if nothing is found, "smallest object" points straight ahead

    for (idx = 1; idx < 91; idx++)
    {
        diff = distance[idx] - distance[idx - 1];

        if (diff <= -20 || diff >= 20)
        {
            if (first >= 0)
                add_object(ping_distance_cm, data, first, idx - 1);

            if (diff <= -20 && distance[idx] < 300.0f)
                first = idx;
            else
                first = -1;
        }
    }
}

int find_smallest(object_data *data)
{
    int k, smallest = 0;
    double min = 1e9;

    for (k = 1; k <= data->object_count; k++)
    {
        if (data->width[k] < min)
        {
            min = data->width[k];
            smallest = k;
        }
    }
    return smallest;
}

void point_to_smallest_obj(int smallest_obj_angle)
{
    servo_move(smallest_obj_angle);
}
