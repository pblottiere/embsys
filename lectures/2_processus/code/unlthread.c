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
        pthread_detach(thread);

        number_of_thread ++;
        usleep(100000);

        if (number_of_thread % 100 == 0)
        {
            printf("Number of threads: %d\n", number_of_thread);
        }
    }

    printf("Number of threads: %d\n", number_of_thread);

    return EXIT_SUCCESS;
}
