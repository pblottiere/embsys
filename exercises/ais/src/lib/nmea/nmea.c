#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "nmea.h"

//-----------------------------------------------------------------------------
int current_date_str(char * str)
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    sprintf(str, "%d%d%d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

//-----------------------------------------------------------------------------
int decimal_to_str(float decimal, size_t size, char * format, char *str)
{
    int deg = floor(decimal);
    float min = (decimal - deg) * 60;
    float coord = deg*100 + min;

    snprintf(str, size, format, coord);

    return coord;
}

//-----------------------------------------------------------------------------
int nmea_gll(struct NMEA_GLL *gll)
{
    // set to zero
    memset(&gll->frame[0], 0, NMEA_GLL_SIZE);

    // header
    memcpy(&gll->frame[0], NMEA_HEADER, 3);

    // frame name
    memcpy(&gll->frame[3], NMEA_FRAME_GLL, 3);

    // separator
    gll->frame[6] = NMEA_SEPARATOR;

    // latitude
    char lat_str[NMEA_LAT_SIZE];
    float lat = decimal_to_str(gll->latitude, NMEA_LAT_SIZE, "%07.2f", lat_str);
    memcpy(&gll->frame[7], lat_str, NMEA_LAT_SIZE);

    // separator
    gll->frame[14] = NMEA_SEPARATOR;

    // latitude orientation
    if (lat < 0)
        gll->frame[15] = 'S';
    else
        gll->frame[15] = 'N';

    // separator
    gll->frame[16] = NMEA_SEPARATOR;

    // longitude
    char long_str[NMEA_LAT_SIZE];
    float lon = decimal_to_str(gll->longitude, NMEA_LONG_SIZE, "%08.2f",
                                long_str);
    memcpy(&gll->frame[17], long_str, NMEA_LONG_SIZE);

    // separator
    gll->frame[25] = NMEA_SEPARATOR;

    // longitude orientation
    if (lon < 0)
        gll->frame[26] = 'W';
    else
        gll->frame[26] = 'E';

    // separator
    gll->frame[27] = NMEA_SEPARATOR;

    // date
    char date_str[NMEA_DATE_SIZE];
    current_date_str(date_str);
    memcpy(&gll->frame[28], date_str, NMEA_DATE_SIZE);

    // separator
    gll->frame[34] = NMEA_SEPARATOR;

    // valid
    gll->frame[35] = 'A';

    return 0;
}
