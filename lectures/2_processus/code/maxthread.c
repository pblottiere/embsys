#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void * function(void * arg) { return NULL;  }

int main()
{
    pthread_t thread;
    int number_of_thread = 0;

    while(pthread_create(&thread, NULL, function, NULL) == 0)
    {
        number_of_thread ++;
        usleep(100000);
    }

    printf("Number of threads: %d\n", number_of_thread);

    return EXIT_SUCCESS;
}
