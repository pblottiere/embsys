//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <ptmx.h>
#include <nmea.h>

//-----------------------------------------------------------------------------
struct PTMX ptmx;

float lat = 48.608958;
float lon = 7.682288;
float course_real = 54.7;
float course_magn = 34.4;
float speed_knot = 5.5;

//-----------------------------------------------------------------------------
void signals_handler(int signal_number)
{
    printf("Signal catched.\n");
    ptmxclose(&ptmx);
    exit(EXIT_SUCCESS);
}

//-----------------------------------------------------------------------------
int write_vtg(int fd)
{
    size_t bytes = 0;

    struct NMEA_VTG vtg;
    vtg.course_real = course_real;
    vtg.course_magn = course_magn;
    vtg.speed_knot = speed_knot;

    if ( nmea_vtg(&vtg) == -1)
    {
        perror("nmea_vtg");
        exit(EXIT_FAILURE);
    }
    else
    {
        bytes = write(fd, vtg.frame, sizeof(vtg.frame));
    }

    course_real += 0.01;
    course_magn += 0.01;
    speed_knot += 0.01;

    return bytes;
}

//-----------------------------------------------------------------------------
int write_gll(int fd)
{
    size_t bytes = 0;

    struct NMEA_GLL gll;
    gll.latitude = lat;
    gll.longitude = lon;

    if ( nmea_gll(&gll) == -1)
    {
        perror("nmea_gll");
        exit(EXIT_FAILURE);
    }
    else
    {
        bytes = write(fd, gll.frame, sizeof(gll.frame));
    }

    lat += 0.00001;
    lon += 0.00001;

    return bytes;
}

//-----------------------------------------------------------------------------
int main()
{
    // open virtual serial port
    if ( ptmxopen(&ptmx) == -1)
    {
        fprintf(stderr, "Error on ptmxopen(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // signals handler
    struct sigaction action;
    action.sa_handler = signals_handler;
    sigemptyset(& (action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGINT, & action, NULL);

    // print
    printf("PTTY: %s\n", ptmx.port);

    // work
    while(1)
    {
        sleep(2);
        write_gll(ptmx.fd);

        sleep(2);
        write_vtg(ptmx.fd);
    }

    ptmxclose(&ptmx);

    return EXIT_SUCCESS;
}
