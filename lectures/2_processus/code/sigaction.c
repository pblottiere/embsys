#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signals_handler(int signal_number)
{
    printf("Signal %d received\n", signal_number);
}

int main()
{
    struct sigaction action;

    action.sa_handler = signals_handler;
    sigemptyset(& (action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGQUIT, & action, NULL);

    action.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, & action, NULL);

    while(1)
    {
        printf("sleep...\n");
        sleep(2);
    }

    return EXIT_SUCCESS;
}
