#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <syslog.h>

#include <util.h>

void logger()
{
    // open log file with name, header options for each message written, and facility
    openlog("test", LOG_PID | LOG_NDELAY, LOG_LOCAL0);

    // log a message
    syslog(LOG_LOCAL0 | LOG_INFO, "PPID %d", getppid());
    syslog(LOG_LOCAL0 | LOG_INFO, "PID %d", getpid());
    syslog(LOG_LOCAL0 | LOG_DEBUG, "Testing!");

    closelog();
}

//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    int n = 2;
    char *ports[2];
    char *port = NULL;

    // parse comand line
    if (argc != 3 + n)
    {
        fprintf(stderr, "Invalid usage: reader -p port_name1 -q port_name2\n");
        exit(EXIT_FAILURE);
    }

    char *options = "p:q:";
    int option;
    while ((option = getopt(argc, argv, options)) != -1)
    {
        switch (option)
        {
        case 'p':
            port = optarg;
            ports[0] = optarg;
            break;
        case 'q':
            port = optarg;
            ports[1] = optarg;
            break;

        case '?':
            fprintf(stderr, "Invalid option %c\n", optopt);
            exit(EXIT_FAILURE);
        }
    }

    // open serial port
    int fds[2];
    for (int i = 0; i < n; i++)
    {
        printf("%s\n", ports[i]);
        int fd = open(ports[i], O_RDWR | O_NOCTTY);
        if (fd == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        tcflush(fd, TCIOFLUSH);
        fds[i] = fd;
        printf("fd = %i \n", fd);
    }

    //call to the logger function
    logger();

    // read port
    char buff[50];
    fd_set fdset;

    while (1)
    {
        bzero(buff, sizeof(buff));

        FD_ZERO(&fdset);
        int fd;
        for (int i = 0; i < n; i++)
        {
            fd = fds[i];
            FD_SET(fd, &fdset);

            select(fd + 1, &fdset, NULL, NULL, NULL);

            if (FD_ISSET(fd, &fdset))
            {
                int bytes = read(fd, buff, sizeof(buff));

                if (bytes > 0)
                {
                    printf("GPS fd = %i ------- %s\n", fd, buff);
                    fflush(stdout);
                }
            }
        }
    }

    // close serial port
    for (int i = 0; i < n; i++)
        close(fds[i]);

    exit(EXIT_SUCCESS);
}
