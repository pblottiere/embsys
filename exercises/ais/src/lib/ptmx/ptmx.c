#define _XOPEN_SOURCE 600

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <termios.h>

#include "ptmx.h"

int ptmxopen(struct PTMX *ptmx)
{
    // open file descriptor
    int fd = posix_openpt(O_RDWR | O_NOCTTY);

    if (fd == -1)
        return -1;

    // grant access
    int rc = grantpt(fd);
    if (rc == -1)
        return -1;

    rc = unlockpt(fd);
    if (rc == -1)
        return -1;

    struct termios newtio, oldtio;
    tcgetattr(fd, &oldtio);

    newtio.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    newtio.c_oflag &= ~OPOST;
    newtio.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
    newtio.c_cflag &= ~(CSIZE|PARENB);
    newtio.c_cflag |= CS8;
    // newtio.c_cflag |= B38400;

    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 10;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    // store in structure
    ptmx->fd = fd;
    ptmx->origintio = newtio;
}
