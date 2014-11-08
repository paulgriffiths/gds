#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <assert.h>
#include "gds_util.h"

void gds_strerror_quit(const char * msg, ...)
{
    fprintf(stderr, "gds library error: %s (%d)\n", strerror(errno), errno);
    fprintf(stderr, "gds library error: ");

    va_list ap;
    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    va_end(ap);

    if ( msg[strlen(msg) - 1] != '\n' ) {
        fputc('\n', stderr);
    }    

    exit(EXIT_FAILURE);
}

void gds_error_quit(const char * msg, ...)
{
    fprintf(stderr, "gds library error: ");

    va_list ap;
    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    va_end(ap);

    if ( msg[strlen(msg) - 1] != '\n' ) {
        fputc('\n', stderr);
    }    

    exit(EXIT_FAILURE);
}

void gds_assert_quit(const char * msg, ...)
{

#ifndef NDEBUG

    fprintf(stderr, "gds library error: ");

    va_list ap;
    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    va_end(ap);

    if ( msg[strlen(msg) - 1] != '\n' ) {
        fputc('\n', stderr);
    }    

#endif

    assert(false);
}
