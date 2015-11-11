#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

//-----------------------------------------------------------------------------
int main(int argc, char *argv [])
{
    char * port;

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

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    }

    // open serial port
    int fd = open(port, O_RDWR | O_NOCTTY);
    if (fd == -1)
    {
        fprintf(stderr, "Error on open(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // read port
    char buff[50];

    while(1)
    {
        bzero(&buff, sizeof(buff));
        size_t bytes = read(fd, &buff, sizeof(buff));

        if (bytes > 0)
        {
            printf("buff (%d): %s\n", bytes, buff);
        }
    }

    // close serial port
    close(fd);

    exit(EXIT_SUCCESS);
}
