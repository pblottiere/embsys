#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "nmea.h"

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
        puts(NULL);
    }
#endif

    return kmh;
}

//-----------------------------------------------------------------------------
int current_date_str(char * str)
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    sprintf(str, "%02d%02d%02d", timeinfo->tm_hour, timeinfo->tm_min,
            timeinfo->tm_sec);
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

//-----------------------------------------------------------------------------
int nmea_vtg(struct NMEA_VTG *vtg)
{
    // set to zero
    memset(&vtg->frame[0], 0, NMEA_VTG_SIZE);

    // header
    memcpy(&vtg->frame[0], NMEA_HEADER, 3);

    // frame name
    memcpy(&vtg->frame[3], NMEA_FRAME_VTG, 3);

    // separator
    vtg->frame[6] = NMEA_SEPARATOR;

    // course real
    char course_real_str[NMEA_COURSE_SIZE];
    snprintf(course_real_str, NMEA_COURSE_SIZE, "%05.1f", vtg->course_real);
    memcpy(&vtg->frame[7], course_real_str, NMEA_COURSE_SIZE);

    // separator
    vtg->frame[12] = NMEA_SEPARATOR;

    // true track
    vtg->frame[13] = 'T';

    // separator
    vtg->frame[14] = NMEA_SEPARATOR;

    // course magnetic
    char course_magn_str[NMEA_COURSE_SIZE];
    snprintf(course_magn_str, NMEA_COURSE_SIZE, "%05.1f", vtg->course_magn);
    memcpy(&vtg->frame[15], course_magn_str, NMEA_COURSE_SIZE);

    // separator
    vtg->frame[20] = NMEA_SEPARATOR;

    // true track
    vtg->frame[21] = 'M';

    // separator
    vtg->frame[22] = NMEA_SEPARATOR;

    // not speed
    char speed_knot_str[NMEA_SPEED_SIZE];
    snprintf(speed_knot_str, NMEA_SPEED_SIZE, "%05.1f", vtg->speed_knot);
    memcpy(&vtg->frame[23], speed_knot_str, NMEA_SPEED_SIZE);

    // separator
    vtg->frame[28] = NMEA_SEPARATOR;

    // true track
    vtg->frame[29] = 'N';

    // separator
    vtg->frame[28] = NMEA_SEPARATOR;

    // kmh speed
    char speed_kmh_str[NMEA_SPEED_SIZE];
    knot_to_kmh_str(vtg->speed_knot, NMEA_SPEED_SIZE, "%05.1f", speed_kmh_str);
    memcpy(&vtg->frame[29], speed_kmh_str, NMEA_SPEED_SIZE);

    // separator
    vtg->frame[34] = NMEA_SEPARATOR;

    // true track
    vtg->frame[35] = 'K';

    return 0;
}
