#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define KNOT_TO_KMH 1.852

int iteration = 0;

// Question 8 : write here the buggy function without errors
//-----------------------------------------------------------------------------
int knot_to_kmh_str(float not, size_t size, char *format, char *kmh_str)
{
    float kmh = KNOT_TO_KMH * not ;
    snprintf(kmh_str, size, format, kmh);

    puts("Waiting for GPS...");

#ifndef GPS_OK
    iteration++;
    if (iteration == 2)
    {
        puts("GPS OK");
    }
#endif

    return kmh;
}
// Question 12 : write printf with a signal handler
//-----------------------------------------------------------------------------
void signal_handler(int signal_number)
{
    fprintf(stdout, "Signal SIGINT catched by the hook.\n");
}

extern int printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int return_status = 0;
    return_status = vfprintf(stdout, format, args);
    // signals handler
    struct sigaction action;
    action.sa_handler = signal_handler;
    sigemptyset(&(action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);
    va_end(args);
    return return_status;
}