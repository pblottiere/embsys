
#include <stdio.h>

#define KNOT_TO_KMH 1.852

int iteration = 0;

// Question 8 : write here the buggy function without errors
int knot_to_kmh_str(float not, size_t size, char * format, char * kmh_str)
{
    float kmh = KNOT_TO_KMH * not;
    snprintf(kmh_str, size, format, kmh);

#ifndef GPS_OK
    iteration++;
    if (iteration == 2)
    {
        puts(NULL);
    }
#endif

    return (float)kmh;
}
// Question 12 : write printf with a signal handler

