#ifndef OPTS_H
#define OPTS_H

//-----------------------------------------------------------------------------
struct OPTS
{
    char * sem;
	char * shm;
};

//-----------------------------------------------------------------------------
int parse_args(int argc, char *argv[], struct OPTS *opts);

#endif // OPTS_H
