/*!
 * \file            gds_util_string.h
 * \brief           Interface to general utility string functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_STRING_H
#define PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_STRING_H

/*!
 * \brief           Dynamically duplicates a string.
 * \details         Provided in case POSIX `strdup()` is not available.
 * \ingroup         general
 * \param str       The string to duplicate.
 * \retval NULL     Failure, dynamic allocation failed
 * \retval non-NULL A pointer to the new string
 */
char * gds_strdup(const char * str);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_STRING_H  */
