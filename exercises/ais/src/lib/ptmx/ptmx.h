#ifndef PTMX_H
#define PTMX_H

#include <termios.h>

struct PTMX
{
    int fd;
    struct termios origintio;
};

int ptmxopen(struct PTMX *ptmx);

#endif
