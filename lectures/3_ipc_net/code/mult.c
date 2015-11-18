#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    fd_set fdset;
    char buff[255];

    int fd = fileno(stdin);
    if (fd < 0)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    while (1)
    {
        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);
        select(fd+1, &fdset, NULL, NULL, NULL);

        if (FD_ISSET(fd, &fdset))
            fgets(buff, sizeof(buff), stdin);
            printf("EVENT: %s", buff);
    }

    return EXIT_SUCCESS;
}
