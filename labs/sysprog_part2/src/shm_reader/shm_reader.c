#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <opts.h>
#include <handler.h>

struct HANDLERS handlers;

//-----------------------------------------------------------------------------
void *shmreader()
{
    while(1)
    {
        if(handlers.shdata != NULL)
        {
            printf("\n");
            printf("time: %d\n", handlers.shdata->time);
            fflush(stdout);
        }
        usleep(500000);
    }
}

//-----------------------------------------------------------------------------
int main(int argc, char *argv [])
{
    // parse args
    struct OPTS opts;
    if (parse_args(argc, argv, &opts) == -1)
        exit(EXIT_FAILURE);

    // open handlers
    if (hndopen(opts, &handlers) == -1)
        exit(EXIT_FAILURE);

    // run
    shmreader();

    // close
    hndclose(&handlers);

    exit(EXIT_SUCCESS);
}
