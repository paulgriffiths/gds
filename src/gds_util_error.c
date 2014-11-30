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

void gds_logerror_line(const char * progname,
                       const char * filename,
                       const int linenum,
                       const bool log_errno,
                       const enum gds_error_quit_type quit_type,
                       const char * fmt, ...)
{
    FILE * log = gds_errlog();

    fprintf(log, "%s: %s: %s:%d: ", progname,
            (quit_type == GDS_ERROR_ASSERT) ? "assertion failed" : "error",
            filename, linenum);
    if ( log_errno ) {
        fprintf(log, "%s (%d): ", strerror(errno), errno);
    }

    va_list ap;
    va_start(ap, fmt);
    vfprintf(log, fmt, ap);
    va_end(ap);

    if ( fmt[strlen(fmt) - 1] != '\n' ) {
        fputc('\n', log);
    }    

    switch ( quit_type ) {
        case GDS_ERROR_NOQUIT:
            break;

        case GDS_ERROR_EXIT:
            exit(EXIT_FAILURE);
            break;

        case GDS_ERROR_ASSERT:
            abort();
            break;

        default:
            assert(false);
            break;
    }
}
