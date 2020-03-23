// Question 8 : write here the buggy function without errors
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define KNOT_TO_KMH 1.852

int knot_to_kmh_str(float not, size_t size, char * format, char * kmh_str) {
	float kmh = KNOT_TO_KMH * not;
    snprintf(kmh_str, size, format, kmh);

    // Bypassing error
/*
#ifndef GPS_OK
    iteration++;
    if (iteration == 2)
    {
        puts(NULL);
    }
#endif */

    return kmh;
}

// Question 12 : write printf with a signal handler

// void printf_signals_handler(int signal_number) {
//     printf("Signal catched.\n");
// }

// int printf(const char *format, ...) {
// 	// signals handler
//     struct sigaction action;
//     action.sa_handler = printf_signals_handler;
//     sigemptyset(& (action.sa_mask));
//     action.sa_flags = 0;
//     sigaction(SIGINT, & action, NULL);

// 	return 0;
// }