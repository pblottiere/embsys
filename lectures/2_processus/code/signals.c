#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main()
{
    int i;
    for (i = 1; i < NSIG; i++)
        printf("%d : %s\n", i, sys_siglist[i]);

    return EXIT_SUCCESS;
}
