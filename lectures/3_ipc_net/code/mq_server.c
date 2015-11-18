#include <stdlib.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

void send(mqd_t mq, char *msg, int priority)
{
    if (mq_send(mq, msg, strlen(msg), priority) != 0)
    {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    mqd_t mq = mq_open("/MQ_NAME", O_WRONLY|O_CREAT, 0644, NULL);
    if (mq == (mqd_t) -1)
    {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    send(mq, "MSG1", 0);
    send(mq, "MSG2", 3);
    send(mq, "MSG3", 2);

    return EXIT_SUCCESS;
}
