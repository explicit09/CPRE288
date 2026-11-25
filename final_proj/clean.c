#include "clean.h"
#include "stdio.h"
#include "math.h"

void outlier_removal(float distance[])
{
    int l = 0;
    if(fabsf(distance[0] - distance[1]) >= 20
            && fabsf(distance[1] - distance[2]) < 20)
            {
                distance[0] = distance[1];
            }
    
    for (l = 1; l < 90; l++) 
    {
        float current = distance[l]; 
        float previous = distance[l - 1];
        float next = distance[l + 1];

        if (((current - previous >= 20) && (current - next >= 20))
            || ((current - previous <= -20) && (current - next <= -20)))
        {
            //using max and min values
            if (current > previous)
            {
                distance[l] = fmaxf(previous,next);
            }
            else{
                distance[l] = fminf(previous,next);
            }
        }
    }

    if (fabsf(distance[90] - distance[89]) >= 20
            && fabsf(distance[89] - distance[88])
                    < 20)
        distance[90] = distance[89];
}



void moving_average(float distance[])
{
    float window = 3.0;
    float left = 0.0;
    float right = 0.0;
    float temp[91];
    int l;

    for (l = 0; l < 91; l++)
    {
        float itself = distance[l];

        if( l >0)
        {
            left = distance[l-1];
        }
        else{
            left = itself;
        }

        if( l < 90)
        {
            right = distance[l+1];
        }
        else{
            right = itself;
        }
       
        if (fabsf(left - itself) >= 20)
            left = itself;
        if (fabsf(right - itself) >= 20)
            right = itself;

        temp[l] = (left + itself + right) / window;
        distance[l] = temp[l];
    }
        
    }
