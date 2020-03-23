#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#include <util.h>

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

    char * options = "p:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'p':
                port = optarg;
                break;

            case 'd':
                port2 = optarg;
                break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    }

    // open serial port
    int fd = open(port, O_RDWR | O_NOCTTY);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    tcflush(fd, TCIOFLUSH);

    int fd2 = open(port2, O_RDWR | O_NOCTTY);
    if (fd2 == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    tcflush(fd2, TCIOFLUSH);

    // read port
    char buff[50];
    fd_set fdset;
    fd_set2 fdset2;

    while(1)
    {
        bzero(buff, sizeof(buff));

        FD_ZERO(&fdset);
        FD_ZERO(&fdset2);
        FD_SET(fd, &fdset);
        FD_SET(fd2, &fdset2);

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

        select(fd2+1, &fdset2, NULL, NULL, NULL);

        if (FD_ISSET(fd2, &fdset2))
        {
            int bytes2 = read (fd2, buff, sizeof(buff));

            if (bytes2 > 0)
            {
                printf("%s\n", buff);
                fflush(stdout);
            }
        }
    }

    // close serial port
    close(fd);
    close(fd2);

    exit(EXIT_SUCCESS);
}
