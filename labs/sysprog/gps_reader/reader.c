#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <signal.h>
#include <util.h>
#include <syslog.h>


int fd = 0;
int fd2 = 0;

void signals_handler(int signal_number)
{
    printf("Signal catched. Ending communications...\n");
    close(fd);
    close(fd2);
    closelog();
    printf("Done\n");
    exit(EXIT_SUCCESS);
}

//-----------------------------------------------------------------------------
int main(int argc, char *argv [])
{
    char * port = NULL;
    char * port2 = NULL;

    // parse comand line
    if (argc != 5)
    {
        fprintf(stderr, "Invalid usage: reader -p port_name -g second port_name\n");
        exit(EXIT_FAILURE);
    }

    char * options = "p:g:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'p':
                port = optarg;
                break;

	    case 'g':
                port2 = optarg;
                break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    }
	
    // open serial port
    fd = open(port, O_RDWR | O_NOCTTY);
    fd2 = open(port2, O_RDWR | O_NOCTTY);
    openlog("log_gps_reader", LOG_PID | LOG_NDELAY, LOG_USER);
    if (fd == -1 | fd2 == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    struct sigaction action;
    action.sa_handler = signals_handler;
    sigemptyset(& (action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGINT, & action, NULL);
    tcflush(fd, TCIOFLUSH);
    tcflush(fd2, TCIOFLUSH);
    // read port
    char buff[50];
    fd_set fdset;

    while(1)
    {
        bzero(buff, sizeof(buff));

        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);
	FD_SET(fd2, &fdset);

	int num = 0;
	if(fd2 > fd)
	{
		num = fd2+1;
	}
	else
	{
		num = fd+1;
	}
        select(num, &fdset, NULL, NULL, NULL);

        if (FD_ISSET(fd, &fdset))
        {
            int bytes = read (fd, buff, sizeof(buff));

            if (bytes > 0)
            {
                printf("GPS1 : %s\n", buff);
                syslog(LOG_INFO,"GPS1 data received"); 
                fflush(stdout);
            }
        }

        if (FD_ISSET(fd2, &fdset))
        {
            int bytes = read (fd2, buff, sizeof(buff));

            if (bytes > 0)
            {
                printf("GPS2 : %s\n", buff);
		syslog(LOG_INFO,"GPS2 data received"); 
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
