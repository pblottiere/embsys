#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

//=============================================================================
// main
//=============================================================================
int main()
{
    int fd = open("/dev/pts/0", O_RDWR | O_NOCTTY);
    ioctl(fd, TIOCSCTTY, 0);

    if (fd == -1)
    {
        fprintf(stderr, "Error on open(): %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    char buff[50];

    while(1)
    {
        bzero(&buff, sizeof(buff));
        size_t bytes = read(fd, &buff, sizeof(buff));

        if (bytes > 0)
        {
            printf("buff (%d): %s\n", bytes, buff);
        }
    }

    close(fd);

    return EXIT_SUCCESS;
}
