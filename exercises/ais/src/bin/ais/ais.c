#define _XOPEN_SOURCE 600

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <termios.h>

#include <ptmx.h>

void close_ptmx();
void signals_handler(int);

//=============================================================================
// const, enum, ...
//=============================================================================
int fd_ptmx = 0;
struct termios oldtio;

//=============================================================================
// Functions
//=============================================================================
//-----------------------------------------------------------------------------
void exit_error(char *fct)
{
    if ((fd_ptmx != 0) || (fd_ptmx != -1))
        close_ptmx();

    fprintf(stderr, "Error on %s(): %s\n", fct, strerror(errno));
    exit(EXIT_FAILURE);
}

//-----------------------------------------------------------------------------
void close_ptmx()
{
    tcsetattr(fd_ptmx, TCSANOW, &oldtio);
    int rc = close(fd_ptmx);

    if (rc == -1)
        exit_error("close");
}

//-----------------------------------------------------------------------------
void init_signal_handler()
{
    struct sigaction action;

    action.sa_handler = signals_handler;
    sigemptyset(& (action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGINT, & action, NULL);
}

//-----------------------------------------------------------------------------
void signals_handler(int signal_number)
{
    close_ptmx();

    printf("Signal catched.\n");

    exit(EXIT_SUCCESS);
}

//=============================================================================
// main
//=============================================================================
int main()
{
    struct PTMX ptmx;
    ptmxopen(&ptmx);
    fd_ptmx = ptmx.fd;

    // signals handler
    init_signal_handler();

    // print
    char *name = ptsname(fd_ptmx);
    if (name == NULL)
        exit_error("ptsname");
    printf("PTTY: %s\n", name);

    // work
    while(1)
    {
        size_t bytes = write(fd_ptmx, "coucou", sizeof("coucou"));
        printf("bytes: %d\n", bytes);
        sleep(2);
    }

    // close virtual serial port
    close_ptmx();

    return EXIT_SUCCESS;
}
