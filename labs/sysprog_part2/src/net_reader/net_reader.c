#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#include <opts.h>

struct HANDLERS
{
    int sockfd;
    struct sockaddr_in info_me;
};

//-----------------------------------------------------------------------------
int init_socket(int port, struct HANDLERS * handlers)
{
    // init socket
    handlers->sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (handlers->sockfd == -1)
    {
        perror("socket");
        return -1;
    }

    // configure
    bzero((char *)&handlers->info_me, sizeof(handlers->info_me));
    handlers->info_me.sin_family = AF_INET;
    handlers->info_me.sin_port = htons(port);
    handlers->info_me.sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind(handlers->sockfd, (struct sockaddr*) &handlers->info_me,
             sizeof(handlers->info_me)) == -1)
    {
        perror("bind");
        return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // parse args
    struct OPTS opts;
    if (parse_args(argc, argv, &opts) == -1)
        exit(EXIT_FAILURE);

    // init socket
    struct HANDLERS handlers;
    if (init_socket(opts.port, &handlers) == -1)
        exit(EXIT_FAILURE);

    // receive
    char buf[BUFSIZ];
    struct sockaddr_in info_other;
    int len = sizeof(info_other);

    while(1)
    {
        int bytes = recvfrom(handlers.sockfd, buf, BUFSIZ, 0,
                    (struct sockaddr *) &info_other, &len);
        printf("received: %s\n", buf);
        fflush(stdout);
    }

    return EXIT_SUCCESS;
}
