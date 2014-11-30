/*!
 * \file            gds_util_error.c
 * \brief           Implementation of general utility error functions.
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
#include <pggds/gds_util.h>

void gds_strerror_line_quit(const char * progname,
                            const char * filename,
                            const int linenum,
                            const char * fmt, ...)
{
    fprintf(gds_errlog(), "%s: error: ", progname);

    va_list ap;
    va_start(ap, fmt);
    vfprintf(gds_errlog(), fmt, ap);
    va_end(ap);

    if ( fmt[strlen(fmt) - 1] != '\n' ) {
        fputc('\n', gds_errlog());
    }    

    fprintf(gds_errlog(), "  %s:%d: %s (%d)\n", filename, linenum,
            strerror(errno), errno);

    exit(EXIT_FAILURE);
}

void gds_error_line_quit(const char * progname,
                         const char * filename,
                         const int linenum,
                         const char * fmt, ...)
{
    fprintf(gds_errlog(), "%s: error: %s:%d: ", progname, filename, linenum);

    va_list ap;
    va_start(ap, fmt);
    vfprintf(gds_errlog(), fmt, ap);
    va_end(ap);

    if ( fmt[strlen(fmt) - 1] != '\n' ) {
        fputc('\n', gds_errlog());
    }    

    exit(EXIT_FAILURE);
}

void gds_assert_line_quit(const char * progname,
                          const char * filename,
                          const int linenum,
                          const char * fmt, ...)
{
    fprintf(gds_errlog(), "%s: assertion failed: %s:%d: ",
            progname, filename, linenum);

    va_list ap;
    va_start(ap, fmt);
    vfprintf(gds_errlog(), fmt, ap);
    va_end(ap);

    if ( fmt[strlen(fmt) - 1] != '\n' ) {
        fputc('\n', gds_errlog());
    }    

    abort();
}
