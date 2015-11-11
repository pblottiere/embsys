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

//-----------------------------------------------------------------------------
void signals_handler(int signal_number)
{
    printf("Signal catched.\n");
    ptmxclose(&ptmx);
    exit(EXIT_SUCCESS);
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

    // init frame
    struct NMEA_GLL gll;
    gll.latitude = 48.608958;
    gll.longitude = 7.682288;

    if ( nmea_gll(&gll) == -1)
    {
        fprintf(stderr, "Error on nmea_gll(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // print
    printf("PTTY: %s\n", ptmx.port);

    // work
    while(1)
    {
        size_t bytes = write(ptmx.fd, gll.frame, sizeof(gll.frame));
        printf("bytes: %d\n", bytes);
        sleep(2);
    }

    ptmxclose(&ptmx);

    return EXIT_SUCCESS;
}
