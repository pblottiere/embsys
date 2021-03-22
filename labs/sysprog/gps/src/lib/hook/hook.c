#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "hook.h"

#define KNOT_TO_KMH 1.852

int iteration = 0;

//-----------------------------------------------------------------------------
int knot_to_kmh_str(float not, size_t size, char * format, char * kmh_str)
{
    float kmh = KNOT_TO_KMH * not;
    snprintf(kmh_str, size, format, kmh);

#ifndef GPS_OK
    iteration++;
    if (iteration == 2)
    {
        puts("working");
    }
#endif

    return kmh;
}
