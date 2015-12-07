#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <opts.h>

//-----------------------------------------------------------------------------
void print_usage()
{
	fprintf(stderr, "Invalid usage: forwarder -s shm -l sem -p port\n");
}

//-----------------------------------------------------------------------------
int parse_args(int argc, char *argv[], struct OPTS *opts)
{
	char * shm = NULL;
    char * sem = NULL;
    int port = -1;

	// parse comand line
    if (argc != 7)
    {
		print_usage();
		return -1;
    }

    char * options = "l:p:s:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'l':
                sem = optarg;
                break;

            case 'p':
                port = atoi(optarg);
                break;

			case 's':
				shm = optarg;
				break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
				return -1;
        }
    }

	// test opts
	if ((shm == NULL) || (sem == NULL) || (port == -1))
	{
	 	print_usage();
		return -1;
	}

	opts->shm = shm;
    opts->sem= sem;
    opts->port = port;

	return 0;
}
