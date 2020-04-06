#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <signal.h>  //for signal handler
#include <syslog.h>  //for syslog use
#include <util.h>

#define max(x,y) (x>=y?x:y)

int fd = -1, fd2 = -1;
//-----------------------------------------------------------------------------
void signals_handler(int signal_number)
{
	printf("Signal catched. Fermeture des fd !\n");
	close(fd);
	close(fd2);
	closelog();
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
		fprintf(stderr, "Invalid usage: reader -p first_port_name -h second_port_name\n");
		exit(EXIT_FAILURE);
	}

	char * options = "p:h:";
	int option;
	while((option = getopt(argc, argv, options)) != -1)
	{
		switch(option)
		{
			case 'p':
				port = optarg;
				break;
			case 'h':
				port2 = optarg;
				break;
			case '?':
				fprintf(stderr, "Invalid option %c\n", optopt);
				exit(EXIT_FAILURE);
		}
	}
	//open log
	//openlog (const char *ident, int option, int facility);
	openlog ("log_gps_reader_tp_embsys", LOG_PID, LOG_USER);	

	// open serial port
	int fd = open(port, O_RDWR | O_NOCTTY);
	int fd2 = open(port2, O_RDWR | O_NOCTTY);

	if ((fd == -1)|(fd2 == -1))  //erreur douverture
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	tcflush(fd, TCIOFLUSH);
	tcflush(fd2, TCIOFLUSH);

	// read port
	char buff[50];
	fd_set fdset;

//------------------------------------------
	// signals handler
	struct sigaction action;
	action.sa_handler = signals_handler;
	sigemptyset(& (action.sa_mask));
	action.sa_flags = 0;
	sigaction(SIGINT, & action, NULL);
//------------------------------------------
	while(1)
	{
		//printf("fd = %i\n fd2 = %i\n", fd, fd2);
		
		bzero(buff, sizeof(buff));

		FD_ZERO(&fdset);
		FD_SET(fd, &fdset);
		FD_SET(fd2, &fdset);
		//select the max file descriptors	
		/*int x = 0;
		if (fd>fd2) x = fd;
		else x = fd2;*/
		select(max(fd, fd2)+1, &fdset, NULL, NULL, NULL);
		if (FD_ISSET(fd, &fdset))
		{
			int bytes = read (fd, buff, sizeof(buff));

			if (bytes > 0)
			{
				printf("First GPS : %s\n", buff);
				//syslog (int priority, const char *format, ...);
				syslog(LOG_INFO, "First GPS data");
				fflush(stdout);
			}
		}
		if (FD_ISSET(fd2, &fdset))
		{
			int bytes = read (fd2, buff, sizeof(buff));

			if (bytes > 0)
			{
				printf("Second GPS : %s\n", buff);
				syslog(LOG_INFO, "Second GPS data");
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
