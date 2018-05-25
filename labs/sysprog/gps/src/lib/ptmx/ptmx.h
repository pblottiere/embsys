#ifndef PTMX_H
#define PTMX_H

#include <termios.h>

struct PTMX
{
    int fd;
    char *port;
    struct termios origintio;
};

int ptmxclose(struct PTMX *ptmx);
int ptmxopen(struct PTMX *ptmx);

#endif
