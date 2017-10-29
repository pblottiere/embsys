#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <sys/timerfd.h>

#include <handler.h>

//-----------------------------------------------------------------------------
void hndinit(struct HANDLERS *handlers)
{
    handlers->sockfd = -1;
    bzero((char *)&handlers->info_me, sizeof(handlers->info_me));
    handlers->shm = NULL;
    handlers->shmfd = -1;
    handlers->shdata = NULL;
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

    // socket opening
    handlers->sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (handlers->sockfd == -1)
    {
        perror("socket");
        goto err;
    }

    bzero((char *)&handlers->info_me, sizeof(handlers->info_me));
    handlers->info_me.sin_family = AF_INET;
    handlers->info_me.sin_port = htons(opts.port);

    if (inet_aton("127.0.0.1", &handlers->info_me.sin_addr) == 0)
    {
        perror("inet_aton");
        goto err;
    }

    // shared memory opening
    handlers->shm = opts.shm;
    handlers->shmfd = shm_open(opts.shm, O_RDWR, S_IRUSR|S_IWUSR);
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

    handlers->shdata = mmap(NULL, sizeof(handlers->shdata), PROT_READ,
                            MAP_SHARED, handlers->shmfd, 0);
    if (handlers->shdata == MAP_FAILED)
    {
        perror("mmap");
        goto err;
    }

    return 0;

err:
    hndclose(handlers);
    return -1;
}

//-----------------------------------------------------------------------------
void hndclose(struct HANDLERS *handlers)
{
    if(handlers->sockfd != -1)
        close(handlers->sockfd);

    if(handlers->shmfd != -1)
        close(handlers->shmfd);

    hndinit(handlers);
}
