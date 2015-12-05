#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <util.h>

//-----------------------------------------------------------------------------
void print_usage()
{
    fprintf(stderr, "Invalid usage: shm_writer -p port_name -s shm\n");
}

//-----------------------------------------------------------------------------
int parse_args(int argc, char *argv[], struct OPTS *opts)
{
    char * port = NULL;
    char * shm = NULL;

    // parse comand line
    if (argc != 5)
    {
        print_usage();
        return -1;
    }

    char * options = "p:s:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'p':
                port = optarg;
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
    if ((port == NULL) || (shm == NULL))
    {
        print_usage();
        return -1;
    }

    opts->port = port;
    opts->shm = shm;

    return 0;
}
