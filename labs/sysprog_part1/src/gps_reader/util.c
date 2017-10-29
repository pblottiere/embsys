#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <nmea.h>
#include <util.h>

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
int get_time(char * frame, char * time_str)
{
    int time = -1;

    if (is_gll(frame) == 0)
    {
        memcpy(time_str, &frame[28], 6);
        time_str[6] = '\0';

        time = atoi(time_str);
    }

    return time;
}
