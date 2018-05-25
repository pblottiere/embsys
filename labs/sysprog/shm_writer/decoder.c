#include <stdlib.h>
#include <string.h>

#include <nmea.h>
#include <decoder.h>

//-----------------------------------------------------------------------------
int is_gll(char * frame)
{
    int is_gll = 1;

    if (strlen(frame) == NMEA_GLL_SIZE)
    {
        char id[4];
        memcpy(id, &frame[3], 3);
        id[3] = '\0';

        if (strcmp("GLL", id) == 0)
            is_gll = 0;
    }

    return is_gll;
}

//-----------------------------------------------------------------------------
int get_time(char * frame)
{
    int time = -1;

    if (is_gll(frame) == 0)
    {
        char time_str[7];
        memcpy(time_str, &frame[28], 6);
        time_str[6] = '\0';

        time = atoi(time_str);
    }

    return time;
}

//-----------------------------------------------------------------------------
float get_latitude(char * frame)
{
    float latitude = -1;

    if (is_gll(frame) == 0)
    {
        char lat_str[8];
        memcpy(lat_str, &frame[7], 7);
        lat_str[7] = '\0';

        latitude = atof(lat_str);
    }

    return latitude;
}

//-----------------------------------------------------------------------------
float get_longitude(char * frame)
{
    float longitude = -1;

    if (is_gll(frame) == 0)
    {
        char long_str[9];
        memcpy(long_str, &frame[17], 8);
        long_str[8] = '\0';

        longitude = atof(long_str);
    }

    return longitude;
}
