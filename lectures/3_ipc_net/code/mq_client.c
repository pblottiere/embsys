#include <stdlib.h>
#include <mqueue.h>
#include <stdio.h>

char * msg = NULL;
void clean_msg() { free(msg); }

int main()
{
    mqd_t mq = mq_open("/MQ_NAME", O_RDONLY);
    if (mq == (mqd_t) -1)
    {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    struct mq_attr mq_attr;
    if (mq_getattr(mq, &mq_attr) != 0)
    {
        perror("mq_getattr");
        return EXIT_FAILURE;
    }

    msg = calloc(mq_attr.mq_msgsize, sizeof(char));
    atexit(clean_msg);
    int priority;
    if (mq_receive(mq, msg, mq_attr.mq_msgsize, &priority) < 0)
    {
        perror("mq_receive");
        return EXIT_FAILURE;
    }

    printf("msg '%s' with priority '%d'\n", msg, priority);
    return EXIT_SUCCESS;
}
