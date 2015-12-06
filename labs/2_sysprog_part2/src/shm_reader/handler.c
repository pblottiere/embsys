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
    handlers->sem = NULL;
}

//-----------------------------------------------------------------------------
int hndopen(struct OPTS opts, struct HANDLERS *handlers)
{
    // init
    hndinit(handlers);

    // open semaphore
    handlers->sem = sem_open(opts.sem, O_RDWR);
    if (handlers->sem == SEM_FAILED)
    {
        perror ("sem_open");
        goto err;
    }

    // shared memory opening
    // TODO

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

    if (handlers->sem != NULL)
        sem_close(handlers->sem);

    hndinit(handlers);
}
