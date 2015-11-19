#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd = open("/tmp/test.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if (fd < 0)
    {
        fprintf(stderr, "Error on open(): %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    printf("fd: %d\n", fd);

    write(fd, "HEY YOU!!!!\n", 12);
    sleep(30); // just a sleep to observe some thing

    close(fd);

    return EXIT_SUCCESS;
}
