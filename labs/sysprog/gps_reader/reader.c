#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#include <sys/types.h>
#include <signal.h>

#include <util.h>

int fd1, fd2;

void signals_handler(int signal_number){
	printf("\nça va couper\n");
    close(fd1);
    close(fd2);

    exit(EXIT_SUCCESS);
}

//-----------------------------------------------------------------------------
int main(int argc, char *argv []){

	//signal handling
	struct sigaction action;

	action.sa_handler = signals_handler;
	sigemptyset(&(action.sa_mask));
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);


	//code de base
    char * port1 = NULL;
    char * port2 = NULL;

    
	//ce code ne marche pas, je l'ai remplacé par un code bourrin qui marche

    // parse comand line
    if (argc != 5)
    {
        fprintf(stderr, "Invalid usage: reader -p port_name_1 -q port_name_2\n");
        exit(EXIT_FAILURE);
    }

    char * options = "p:q:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'p':
                port1 = optarg;
                break;

            case 'q':
                port2 = optarg;
                break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
                exit(EXIT_FAILURE);

            default :
            	printf("Invalid option %c\n", option);
        }
    }
	/**
    if (argc != 3)
    {
        fprintf(stderr, "Invalid usage: reader -port_name_1 -port_name_2\n");
        exit(EXIT_FAILURE);
    }

    port1 = argv[1];
    port2 = argv[2];
	**/

    // open serial port
    fd1 = open(port1, O_RDWR | O_NOCTTY);
    fd2 = open(port2, O_RDWR | O_NOCTTY);

    if (fd1 == -1)
    {
    	printf("%s\n", port1);
        perror("open 1");
        exit(EXIT_FAILURE);
    }
    if (fd2 == -1)
    {
        perror("open 2");
        exit(EXIT_FAILURE);
    }
    tcflush(fd1, TCIOFLUSH);
    tcflush(fd2, TCIOFLUSH);

    // read port
    char buff[50];
    fd_set fdset;

    while(1)
    {
        bzero(buff, sizeof(buff));

        FD_ZERO(&fdset);
        FD_SET(fd1, &fdset);
        FD_SET(fd2, &fdset);


        select(fd1+1, &fdset, NULL, NULL, NULL);

        if (FD_ISSET(fd1, &fdset))
        {
            int bytes = read (fd1, buff, sizeof(buff));

            if (bytes > 0)
            {
                printf("%s\n", buff);
                fflush(stdout);
            }
        }

        bzero(buff, sizeof(buff));

        //select(fd2+1, &fdset, NULL, NULL, NULL);

        if (FD_ISSET(fd2, &fdset))
        {
            int bytes = read (fd2, buff, sizeof(buff));

            if (bytes > 0)
            {
                printf("%s\n", buff);
                fflush(stdout);
            }
        }
    }

    // close serial port
    close(fd1);
    close(fd2);

    exit(EXIT_SUCCESS);
}
