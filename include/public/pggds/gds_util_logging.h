/*!
 * \file            gds_util_logging.h
 * \brief           Interface to logging functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_LOGGING_H
#define PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_LOGGING_H

#include <stdio.h>
#include <stdbool.h>

/*!
 * \brief           Debug printf macro.
 * \param ...       Arguments suitable for passing to printf()
 */
#ifdef GDSDEBUG
#define DPRINTF(...) fprintf(gds_errlog(), __VA_ARGS__)
#else
#define DPRINTF(...)
#endif

/*!
 * \brief           Returns a pointer to the current log file.
 * \ingroup         logging
 * \returns         A pointer to the current log file.
 */
FILE * gds_errlog(void);

/*!
 * \brief               Starts logging functionality.
 * \details             Prior to calling this function, any calls to
 * `gds_log_msg()` will result in no action.
 * \ingroup             logging
 * \param logfilename   The name of the log file to open for writing, or
 * `NULL` to log to the standard error stream.
 * \param append        Set to `true` to append to an existing log file,
 * or `false` to overwrite it. This parameter is ignored if `logfilename`
 * is `NULL`.
 * \retval true         Success
 * \retval false        Failure, log file could not be opened for writing
 */
bool gds_logging_on(const char * logfilename, const bool append);

/*!
 * \brief           Stops logging functionality.
 * \details         After calling this function, any calls to
 * `gds_log_msg()` will result in no action.
 * \ingroup         logging
 * \retval true     Success
 * \retval false    Log file could not be closed, logging still stopped
 */
bool gds_logging_off(void);

void gds_log_msg(const char * fmt, ...);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_LOGGING_H  */
