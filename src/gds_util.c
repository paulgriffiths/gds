/*!
 * \file            gds_util.c
 * \brief           Implementation of general utility functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

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

char * gds_strdup(const char * str)
{
    const size_t len = strlen(str);
    char * new_str = malloc(len + 1);
    if ( !new_str ) {
        return NULL;
    }
    strcpy(new_str, str);
    return new_str;
}
