#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <signal.h>

#include <util.h>

int fd, fd2;

void signals_handler()
{
    printf("All fd are correctly closed\n");
    // close serial port
    close(fd);
    exit(EXIT_SUCCESS);

}

//-----------------------------------------------------------------------------
int main(int argc, char *argv [])
{
    char * port = NULL;
    char * port2 = NULL;

    // parse comand line
    if (argc != 3)
    {
        fprintf(stderr, "Invalid usage: reader -p port_name\n");
        exit(EXIT_FAILURE);
    }

    char * options = "p:o";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'p':
                port = optarg;
                break;

            case 'o':
                port2 = optarg;
                break;

            case '?':
                fprintf(stderr, "Invalid option %c %c\n", option, optopt);
                exit(EXIT_FAILURE);
        }
    }

    // open serial port
    fd = open(port, O_RDWR | O_NOCTTY);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    tcflush(fd, TCIOFLUSH);

    // read port
    char buff[50];
    fd_set fdset;
    fd_set fdset2;

    // for good terminaison
    struct sigaction action;

    action.sa_handler = signals_handler;
    sigemptyset(& (action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGQUIT, & action, NULL);

    action.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, & action, NULL);

    while(1)
    {
        bzero(buff, sizeof(buff));

        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);

        select(fd+1, &fdset, NULL, NULL, NULL);

        if (FD_ISSET(fd, &fdset))
        {
            int bytes = read (fd, buff, sizeof(buff));

            if (bytes > 0)
            {
                printf("%s\n", buff);
                fflush(stdout);
            }
        }
    }


    exit(EXIT_SUCCESS);
}
