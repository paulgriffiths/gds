/*!
 * \file            gds_util_logging.c
 * \brief           Implementation of logging functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <pggds/gds_util.h>

/*!  File scope variable to hold current error file pointer  */
static FILE * gds_error_file = NULL;

/*!  File scope variable to hold current error file name  */
static char * gds_error_file_name = NULL;

/*!  File scope variable for current logging status  */
static bool gds_logging_enabled = false;

FILE * gds_errlog(void)
{
    return gds_error_file ? gds_error_file : stderr;
}

bool gds_logging_on(const char * logfilename, const bool append)
{
    FILE * fp = NULL;
    if ( logfilename ) {
        const char * mode;
        if ( append ) {
            mode = "a";
        }
        else {
            mode = "w";
        }

        if ( !(fp = fopen(logfilename, mode)) ) {
            return false;
        }

        /*  No need to check for failure here, as only required
         *  response is to set gds_error_file_name to NULL,
         *  which happens on failure anyway.                     */

        gds_error_file_name = gds_strdup(logfilename);
    }

    gds_logging_enabled = true;
    gds_error_file = fp;
    return true;
}

bool gds_logging_off(void)
{
    bool close_issue = false;

    if ( gds_error_file ) {
        if ( fclose(gds_error_file) == EOF ) {
            close_issue = true;
            gds_error_file = NULL;

            if ( gds_error_file_name ) {
                log_strerror("gds library", "couldn't close log file %s",
                             gds_error_file_name);
            }
            else {
                log_strerror("gds library", "couldn't close log file");
            }
        }
        else {
            gds_error_file = NULL;
        }

        if ( gds_error_file_name ) {
            free(gds_error_file_name);
            gds_error_file_name = NULL;
        }
    }

    gds_logging_enabled = false;
    return close_issue;
}

void gds_log_msg(const char * fmt, ...)
{
    if ( gds_logging_enabled ) {
        va_list ap;
        va_start(ap, fmt);
        vfprintf(gds_errlog(), fmt, ap);
        va_end(ap);

        if ( fmt[strlen(fmt) - 1] != '\n' ) {
            fputc('\n', gds_errlog());
        }    
    }
}
