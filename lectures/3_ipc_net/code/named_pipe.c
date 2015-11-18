#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    if (mkfifo("MY_PIPE_NAME", 0644) != 0)
    {
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    // unlink("MY_PIPE_NAME");

    return EXIT_SUCCESS;
}
