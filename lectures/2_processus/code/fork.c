#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    pid_t pid_fork = fork();
    if (pid_fork == -1)
    {
        printf("fork error (%s)", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pid_fork == 0)
    {
        printf("Child: PID=%ld / PPID=%ld\n", (long) getpid(), (long) getppid());
        return EXIT_SUCCESS;
    }
    else
    {
        printf("Father: PID=%ld / PPID=%ld\n", (long) getpid(), (long) getppid());
        wait(NULL);
        return EXIT_SUCCESS;
    }
}
