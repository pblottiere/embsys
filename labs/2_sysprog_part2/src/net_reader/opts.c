#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <opts.h>

//-----------------------------------------------------------------------------
void print_usage()
{
    fprintf(stderr, "Invalid usage: net_reader -p port\n");
}

//-----------------------------------------------------------------------------
int parse_args(int argc, char *argv[], struct OPTS *opts)
{
    int port = -1;

    // parse comand line
    if (argc != 3)
    {
        print_usage();
        return -1;
    }

    char * options = "p:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'p':
                port = atoi(optarg);
                break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
                return -1;
        }
    }

    // test opts
    if (port == -1)
    {
        print_usage();
        return -1;
    }

    opts->port = port;

    return 0;
}
