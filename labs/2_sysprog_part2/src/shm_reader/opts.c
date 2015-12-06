#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <opts.h>

//-----------------------------------------------------------------------------
void print_usage()
{
	fprintf(stderr, "Invalid usage: forwarder -s shm\n");
}

//-----------------------------------------------------------------------------
int parse_args(int argc, char *argv[], struct OPTS *opts)
{
	char * shm = NULL;

	// parse comand line
    if (argc != 3)
    {
		print_usage();
		return -1;
    }

    char * options = "s:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
			case 's':
				shm = optarg;
				break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
				return -1;
        }
    }

	// test opts
	if (shm == NULL)
	{
		print_usage();
		return -1;
	}

	opts->shm = shm;

	return 0;
}
