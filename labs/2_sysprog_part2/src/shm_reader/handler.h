#ifndef HANDLER_H
#define HANDLER_H

#include <shm_writer/shdata.h>
#include <semaphore.h>

#include <opts.h>

//-----------------------------------------------------------------------------
struct HANDLERS
{
    sem_t * sem;
    char * shm;
    int shmfd;
    struct SHDATA *shdata;
};

//-----------------------------------------------------------------------------
int hndopen(struct OPTS opts, struct HANDLERS *handlers);
void hndclose(struct HANDLERS *handlers);

#endif // HANDLER_H
