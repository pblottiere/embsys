#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

#include <handler.h>

//-----------------------------------------------------------------------------
void hndinit(struct HANDLERS *handlers)
{
    handlers->gpsfd = -1;
    handlers->sem = NULL;
    handlers->semname = NULL;
    handlers->shmfd = -1;
    handlers->shm = NULL;
    handlers->shdata = NULL;
}

//-----------------------------------------------------------------------------
int hndopen(struct OPTS opts, struct HANDLERS *handlers)
{
    // init handlers
    hndinit(handlers);

    // open semaphore
    handlers->semname = opts.sem;
    handlers->sem = sem_open(opts.sem, O_RDWR|O_CREAT,
                             S_IRUSR|S_IWUSR, 1);
    if (handlers->sem == SEM_FAILED)
    {
        perror ("sem_open");
        goto err;
    }

    // open gps port
    handlers->gpsfd = open(opts.port, O_RDWR|O_NOCTTY);
    if (handlers->gpsfd == -1)
    {
        perror("open");
        goto err;
    }
    tcflush(handlers->gpsfd, TCIOFLUSH);

    // open shared memory and projection
    handlers->shm = opts.shm;
    handlers->shmfd = shm_open(opts.shm, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if (handlers->shmfd == -1)
    {
        perror("shm_open");
        goto err;
    }

    if (ftruncate(handlers->shmfd, sizeof(handlers->shdata)) != 0)
    {
        perror("ftruncate");
        goto err;
    }

    handlers->shdata = mmap(NULL, sizeof(handlers->shdata),
                            PROT_READ|PROT_WRITE, MAP_SHARED, handlers->shmfd, 0);
    if (handlers->shdata == MAP_FAILED)
    {
        perror("mmap");
        goto err;
    }

    return EXIT_SUCCESS;

err:
    hndclose(handlers);
    return EXIT_FAILURE;
}

//-----------------------------------------------------------------------------
int hndclose(struct HANDLERS *handlers)
{
    // close gps port
    if (handlers->gpsfd != -1)
        close(handlers->gpsfd);

    // close semaphore
    if (handlers->sem != NULL)
        sem_close(handlers->sem);

    if (handlers->semname != NULL)
        sem_unlink(handlers->semname);

    // close shm
    if (handlers->shmfd != -1)
        close(handlers->shmfd);

    if (handlers->shm != NULL)
        shm_unlink(handlers->shm);

    // reinit
    hndinit(handlers);

    return 0;
}
