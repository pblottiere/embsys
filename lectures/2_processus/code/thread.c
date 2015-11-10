#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void * function(void *arg)
{
    while(1)
    {
        printf("Secondary thread\n");
        sleep(2);
    }
}

int main()
{
    pthread_t thread;
    if (pthread_create(& thread, NULL, function, NULL) != 0)
    {
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        printf("Main thread\n");
        sleep(2);
    }

    return EXIT_SUCCESS;
}
