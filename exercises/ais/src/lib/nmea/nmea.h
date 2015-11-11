#ifndef NMEA_H
#define NMEA_H

//-----------------------------------------------------------------------------
#define NMEA_LAT_SIZE 8
#define NMEA_LONG_SIZE 9
#define NMEA_DATE_SIZE 7

#define NMEA_HEADER "$GP"
#define NMEA_SEPARATOR ','

#define NMEA_FRAME_GLL "GLL"
#define NMEA_GLL_SIZE 36

//-----------------------------------------------------------------------------
struct COORD_SEXAGECIMAL
{
    int lat_deg;
    int lat_min;
    int lat_sec;
    char lat_orient;
    int long_deg;
    int long_min;
    int long_sec;
    char long_orient;
};

struct NMEA_GLL
{
    char frame[NMEA_GLL_SIZE];
    float latitude; // decimal
    float longitude; // decimal
};

//-----------------------------------------------------------------------------
int nmea_gll(struct NMEA_GLL *gll);

#endif
