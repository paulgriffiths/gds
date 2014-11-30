/*
 * logging
 * =======
 *
 * Demonstration program for GDS logging functionality.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <pggds/gds_util.h>

#define BUFFER_SIZE 100 

void print_help_message(void)
{
    printf("Usage: logging [FILE]\n\n");
    printf("Options:\n");
    printf("  -h, --help       show this help message.\n\n");
    printf("With no FILE, log to standard error.\n");
}

int main(int argc, char ** argv)
{
    char * logfilename = NULL;

    if ( argc == 2 ) {
        if ( !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help") ) {
            print_help_message();
            return EXIT_SUCCESS;
        }
        else {
            logfilename = argv[1];
        }
    }
    else if ( argc > 2 ) {
        print_help_message();
        return EXIT_SUCCESS;
    }

    gds_log_msg("This should not be logged");
    gds_log_msg("This should not be logged with %d number", 1);
    errno = 4;
    log_strerror("logging", "This should always be %s", "logged");

    if ( !gds_logging_on(logfilename, true) ) {
        quit_error("logging", "couldn't start logging");
    }

    time_t now = time(NULL);
    if ( now == -1 ) {
        quit_strerror("logging", "couldn't get current time");
    }

    struct tm * now_tm = localtime(&now);
    if ( !now_tm ) {
        quit_strerror("logging", "couldn't get local time");
    }

    gds_log_msg("Logging started at %s", asctime(now_tm));

    gds_log_msg("This should be logged");
    gds_log_msg("This should be logged with %.2f number", 3.14);
    errno = 5;
    log_strerror("logging", "This should always be %s", "logged");

    gds_logging_off();

    gds_log_msg("This should not be logged");
    gds_log_msg("This should not be logged with %d number", 1);
    errno = 6;
    log_strerror("logging", "This should always be %s", "logged");

    return 0;
}
