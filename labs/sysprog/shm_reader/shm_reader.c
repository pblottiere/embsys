#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <opts.h>
#include <handler.h>

struct HANDLERS handlers;

//-----------------------------------------------------------------------------
void signals_handler(int signal_number)
{
    printf("Signal catched.\n");
    hndclose(&handlers);
    exit(EXIT_SUCCESS);
}

//-----------------------------------------------------------------------------
void *shmreader()
{
    while (1)
    {
        if (handlers.shdata != NULL)
        {
            printf("\n");
            sem_wait(handlers.sem); //wait for the semaphore to be unlocked (value 1)
            printf("time: %d\n", handlers.shdata->time);
            printf("latitude: %f\n", handlers.shdata->latitude);
            printf("longitude: %f\n", handlers.shdata->longitude);
            sem_post(handlers.sem); //lock the semaphor (value 0)
            fflush(stdout);
        }
        usleep(1000000);
    }
}

//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // parse args
    struct OPTS opts;
    if (parse_args(argc, argv, &opts) == -1)
        exit(EXIT_FAILURE);

    // open handlers
    if (hndopen(opts, &handlers) == -1)
        exit(EXIT_FAILURE);

    // signals handler
    struct sigaction action;
    action.sa_handler = signals_handler;
    sigemptyset(&(action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);

    // run
    shmreader();

    // close
    hndclose(&handlers);

    exit(EXIT_SUCCESS);
}
