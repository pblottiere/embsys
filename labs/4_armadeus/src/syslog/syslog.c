#include <syslog.h>
#include <unistd.h>

int main()
{
    setlogmask (LOG_UPTO (LOG_NOTICE));

    openlog ("syslogexample", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

    syslog (LOG_NOTICE, "Log sent to syslog by user %d", getuid ());
    syslog (LOG_INFO, "This log is not sent due to logmask...");

    closelog ();
}
