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
void print_usage()
{
    fprintf(stderr, "Invalid usage: gps_reader -f first_port -s second_port\n");
}

//-----------------------------------------------------------------------------
void event(int fd_in, int fd_out, char * buff)
{
    int bytes = read (fd_in, buff, BUFSIZ);

    // printf("serial event for fd=%d (%d): %s\n", fd_in, bytes, buff);

    if (bytes > 0)
    {
        if (is_gll(buff) == 0)
        {
            char time_str[7];
            int time = get_time(buff, time_str);

            write(fd_out, time_str, sizeof(time_str));
        }
    }
}

//-----------------------------------------------------------------------------
int father(int fd1, int fd2, int pipe, char * port1, char * port2)
{
    //
    int maxfd = fd1;
    if (fd2 > maxfd)
        maxfd = fd2;

    // read port
    char buff[BUFSIZ];
    fd_set fdset;

    while(1)
    {
        bzero(buff, sizeof(buff));

        FD_ZERO(&fdset);
        FD_SET(fd1, &fdset);
        FD_SET(fd2, &fdset);

        select(maxfd+1, &fdset, NULL, NULL, NULL);

        if (FD_ISSET(fd1, &fdset))
            event(fd1, pipe, buff);

        if (FD_ISSET(fd2, &fdset))
            event(fd2, pipe, buff);

        fflush(stdout);
    }
}

//-----------------------------------------------------------------------------
int child(int fd)
{
    // read port
    char buff[BUFSIZ];
    fd_set fdset;

    while(1)
    {
        bzero(buff, sizeof(buff));

        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);

        select(fd+1, &fdset, NULL, NULL, NULL);

        if (FD_ISSET(fd, &fdset))
        {
            int bytes = read (fd, buff, sizeof(buff));

            // printf("pipe event: %s\n", buff);

            if (bytes == 7)
            {
                char time_str_h[3];
                memcpy(time_str_h, &buff[0], 2);
                time_str_h[2] = '\0';

                char time_str_m[3];
                memcpy(time_str_m, &buff[2], 2);
                time_str_m[2] = '\0';

                char time_str_s[3];
                memcpy(time_str_s, &buff[4], 2);
                time_str_s[2] = '\0';

                char message[BUFSIZ];
                sprintf(message, "Heure: %sh %smin %ssec", time_str_h,
                        time_str_m, time_str_s);

                syslog(LOG_INFO, message);
            }
        }
    }
}

//-----------------------------------------------------------------------------
int main(int argc, char *argv [])
{
    char * port1 = NULL;
    char * port2 = NULL;

    // parse comand line
    if (argc != 5)
    {
        print_usage();
        exit(EXIT_FAILURE);
    }

    char * options = "f:s:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'f':
                port1 = optarg;
                break;

            case 's':
                port2 = optarg;
                break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    }

    // open serial port
    int fd1 = open(port1, O_RDWR | O_NOCTTY);
    if (fd1 == -1)
    {
        perror("open first port");
        exit(EXIT_FAILURE);
    }
    tcflush(fd1, TCIOFLUSH);

    int fd2 = open(port2, O_RDWR | O_NOCTTY);
    if (fd2 == -1)
    {
        perror("open second port");
        exit(EXIT_FAILURE);
    }
    tcflush(fd2, TCIOFLUSH);

    // open pipe
    int pipe_fds[2];
    if (pipe(pipe_fds) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // open syslog
    openlog("gps_reader", LOG_PERROR | LOG_PID, LOG_LOCAL0 | LOG_MAIL);

    // fork
    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // child
    if (child_pid == 0)
    {
        child(pipe_fds[0]);
    }
    else
    {
        father(fd1, fd2, pipe_fds[1], port1, port2);
    }

    // close serial port
    close(fd1);
    close(fd2);

    // close syslog
    closelog();

    exit(EXIT_SUCCESS);
}
