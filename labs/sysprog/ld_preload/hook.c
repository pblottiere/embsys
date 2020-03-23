// Question 8 : write here the buggy function without errors

#include <stdio.h>
#include <stdlib.h>

#define KNOT_TO_KMH 1.852

int knot_to_kmh_str(float not, size_t size, char * format, char * kmh_str) {
	float kmh = KNOT_TO_KMH * not;
    snprintf(kmh_str, size, format, kmh);

    // Bypassing error
/*
#ifndef GPS_OK
    iteration++;
    if (iteration == 2)
    {
        puts(NULL);
    }
#endif */

    return kmh;
}

// Question 12 : write printf with a signal handler

