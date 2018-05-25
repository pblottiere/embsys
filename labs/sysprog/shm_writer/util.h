#ifndef UTIL_H
#define UTIL_H

//-----------------------------------------------------------------------------
struct OPTS
{
    char * port;
    char * shm;
    char * sem;
};

//-----------------------------------------------------------------------------
int parse_args(int argc, char *argv[], struct OPTS *opts);

#endif // UTIL_H
