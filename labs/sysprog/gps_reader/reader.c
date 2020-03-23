#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <syslog.h>

#include <util.h>

//-----------------------------------------------------------------------------
int main(int argc, char *argv [])
{
    char * port = NULL;
    char * port2 = NULL;

    // parse comand line
    if (argc < 4)
    {
        fprintf(stderr, "Invalid usage: reader -p port_name\n");
        exit(EXIT_FAILURE);
    }

    char * options = "p:q:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'p':
                port = optarg;
                printf("%s \n", port);
                break;

            case 'q':
                port2 = optarg;
                printf("%s \n", port2);
                break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    }

    // Log
    char logf[] = "log_gps";
    openlog(logf, LOG_CONS|LOG_PID, LOG_LOCAL7);

    // open serial port
    int fd = open(port, O_RDWR | O_NOCTTY);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    tcflush(fd, TCIOFLUSH);

    syslog(LOG_DEBUG, "First port correctly opened !");

    int fd2 = open(port2, O_RDWR | O_NOCTTY);
    if (fd2 == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    tcflush(fd2, TCIOFLUSH);

    syslog(LOG_DEBUG, "First port correctly opened !");

    // read port
    char buff[50];
    fd_set fdset;


    while(1)
    {
        bzero(buff, sizeof(buff));

        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);
        FD_SET(fd2, &fdset);

        select(fd+1, &fdset, NULL, NULL, NULL);

        if (FD_ISSET(fd, &fdset))
        {
            int bytes = read (fd, buff, sizeof(buff));

            if (bytes > 0) {
                printf("%s\n", buff);
                syslog(LOG_DEBUG,"%s", buff);
                fflush(stdout);
            }
        }
    }

    // close serial port
    close(fd);
    close(fd2);
    closelog();

    exit(EXIT_SUCCESS);
}
