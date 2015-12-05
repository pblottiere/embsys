#ifndef HANDLER_H
#define HANDLER_H

#include <util.h>
#include <shdata.h>

//-----------------------------------------------------------------------------
struct HANDLERS
{
    int gpsfd;
    int shmfd;
    char * shm;
    struct SHDATA *shdata;
};

//-----------------------------------------------------------------------------
int hndopen(struct OPTS opts, struct HANDLERS *handlers);
int hndclose(struct HANDLERS *handlers);

#endif // HANDLER_H
