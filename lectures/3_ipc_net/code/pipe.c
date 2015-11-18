#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int pipe_fds[2];
    char * msg = "WATER";

    if (pipe(pipe_fds) != 0)
    {
        perror("pipe");
        return EXIT_FAILURE;
    }

    close(pipe_fds[0]);

    size_t bytes = write(pipe_fds[1], msg, strlen(msg));
    printf("Bytes: %d\n", bytes);
    close(pipe_fds[1]);

    return EXIT_SUCCESS;
}
