#ifndef OPTS_H
#define OPTS_H

//-----------------------------------------------------------------------------
struct OPTS
{
	char * shm;
};

//-----------------------------------------------------------------------------
int parse_args(int argc, char *argv[], struct OPTS *opts);

#endif // OPTS_H
