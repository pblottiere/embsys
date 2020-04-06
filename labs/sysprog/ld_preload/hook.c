// Question 8 : write here the buggy function without errors

// Question 12 : write printf with a signal handler

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

    return kmh;
}