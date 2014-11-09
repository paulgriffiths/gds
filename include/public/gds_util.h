/*!
 * \file            gds_util.h
 * \brief           Interface to general utility functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_H
#define PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_H

/*!
 * \brief           Prints an error message with error number and exits.
 * \ingroup         general
 * \details         This function can be called to print an error message
 * and quit following a function which has indicated failure and has set
 * `errno`. A message containing the error number and a text representation
 * of that error will be printed, following by the message supplied to
 * the function.
 * \param msg       The format string for the message to print. Format
 * specifiers are the same as the `printf()` family of functions.
 * \param ...       Any arguments to the format string.
 */
void gds_strerror_quit(const char * msg, ...);

/*!
 * \brief           Prints an error message exits.
 * \ingroup         general
 * \param msg       The format string for the message to print. Format
 * specifiers are the same as the `printf()` family of functions.
 * \param ...       Any arguments to the format string.
 */
void gds_error_quit(const char * msg, ...);

/*!
 * \brief           Prints an error message exits via assert().
 * \ingroup         general
 * \details         This function will do nothing if `NDEBUG` is defined.
 * Otherwise, it behaves in a manner identical to `gds_error_quit()`
 * except it terminates via `assert()`, rather than `exit()`.
 * \param msg       The format string for the message to print. Format
 * specifiers are the same as the `printf()` family of functions.
 * \param ...       Any arguments to the format string.
 */
void gds_assert_quit(const char * msg, ...);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_H  */
