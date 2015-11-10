#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    pid_t pid_child = getpid();
    pid_t pid_father = getppid();

    printf("PID: %ld\n", (long) pid_child);
    printf("PPID: %ld\n", (long) pid_father);

    return EXIT_SUCCESS;
}
