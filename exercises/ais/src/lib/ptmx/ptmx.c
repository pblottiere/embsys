#define _XOPEN_SOURCE 600

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>

#include "ptmx.h"

//-----------------------------------------------------------------------------
int ptmxclose(struct PTMX *ptmx)
{
    // test if the file descriptor is valid
    if ( fcntl(ptmx->fd, F_GETFD) == -1)
        return -1;

    // reinit the original serial configuration
    tcsetattr(ptmx->fd, TCSANOW, &ptmx->origintio);

    // close the port
    if ( close(ptmx->fd) == -1)
    {
        return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
int ptmxopen(struct PTMX *ptmx)
{
    // open file descriptor
    int fd = posix_openpt(O_RDWR | O_NOCTTY);

    if (fd == -1)
        return -1;

    // grant access
    if (grantpt(fd) == -1)
        return -1;

    if (unlockpt(fd) == -1)
        return -1;

    struct termios newtio, oldtio;
    tcgetattr(fd, &oldtio);

    newtio.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    newtio.c_oflag &= ~OPOST;
    newtio.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
    newtio.c_cflag &= ~(CSIZE|PARENB);
    newtio.c_cflag |= CS8;

    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 10;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    // print
    char *name = ptsname(fd);
    if (name == NULL)
        return -1;

    // store in structure
    ptmx->fd = fd;
    ptmx->origintio = newtio;
    ptmx->port = name;

    return 0;
}
