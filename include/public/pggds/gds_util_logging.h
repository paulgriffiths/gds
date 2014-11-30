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

/*!
 * \brief           Returns a pointer to the current log file.
 * \ingroup         logging
 * \returns         A pointer to the current log file.
 */
FILE * gds_errlog(void);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_LOGGING_H  */
