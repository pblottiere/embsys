#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/timerfd.h>

#include <handler.h>

//-----------------------------------------------------------------------------
void hndinit(struct HANDLERS *handlers)
{
    handlers->shm = NULL;
    handlers->shmfd = -1;
    handlers->shdata = NULL;
}

//-----------------------------------------------------------------------------
int hndopen(struct OPTS opts, struct HANDLERS *handlers)
{
    // init
    hndinit(handlers);

    return 0;

err:
    hndclose(handlers);
    return -1;
}

//-----------------------------------------------------------------------------
void hndclose(struct HANDLERS *handlers)
{
    if(handlers->shmfd != -1)
        close(handlers->shmfd);

    hndinit(handlers);
}
