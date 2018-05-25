#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <util.h>

//-----------------------------------------------------------------------------
void print_usage()
{
    fprintf(stderr, "Invalid usage: shm_writer -p port_name -s shm -l sem\n");
}

//-----------------------------------------------------------------------------
int parse_args(int argc, char *argv[], struct OPTS *opts)
{
    char * port = NULL;
    char * shm = NULL;
    char * sem = NULL;

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
    if ((port == NULL) || (shm == NULL) || (sem== NULL))
    {
        print_usage();
        return -1;
    }

    opts->port = port;
    opts->shm = shm;
    opts->sem = sem;

    return 0;
}
