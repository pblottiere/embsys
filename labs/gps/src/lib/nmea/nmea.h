#ifndef NMEA_H
#define NMEA_H

//-----------------------------------------------------------------------------
#define NMEA_LAT_SIZE 8
#define NMEA_LONG_SIZE 9
#define NMEA_DATE_SIZE 7
#define NMEA_COURSE_SIZE 6
#define NMEA_SPEED_SIZE 6

#define NMEA_HEADER "$GP"
#define NMEA_SEPARATOR ','

#define NMEA_FRAME_GLL "GLL"
#define NMEA_GLL_SIZE 36

#define NMEA_FRAME_VTG "VTG"
#define NMEA_VTG_SIZE 38

//-----------------------------------------------------------------------------
struct NMEA_GLL
{
    char frame[NMEA_GLL_SIZE];
    float latitude; // decimal
    float longitude; // decimal
};

struct NMEA_VTG
{
    char frame[NMEA_VTG_SIZE];
    float course_real; // deg
    float course_magn; // deg
    float speed_knot;
    float speed_kmh;
};

//-----------------------------------------------------------------------------
int nmea_gll(struct NMEA_GLL *gll);
int nmea_vtg(struct NMEA_VTG *vtg);

#endif
