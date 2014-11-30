/*!
 * \file            gds_util_error.h
 * \brief           Interface to general utility error functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_ERROR_H
#define PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_ERROR_H

#include <stdbool.h>

enum gds_error_quit_type {
    GDS_ERROR_NOQUIT,
    GDS_ERROR_EXIT,
    GDS_ERROR_ABORT,
    GDS_ERROR_ASSERT
};

/*!
 * \brief           Prints an error message with error number.
 * \ingroup         general
 * \details         This macro can be called to print an error message
 * and quit following a function which has indicated failure and has set
 * `errno`. A message containing the error number and a text representation
 * of that error will be printed, following by the message supplied to
 * the function. This function is intended to be called from the corresponding
 * macro.
 * \param prog      The program name to include in the error message.
 * \param ...       Other arguments, the first of which should be a format
 * string suitable for passing to `vprintf()`, optionally followed by any
 * additional arguments specified by the format string.
 */
#define log_strerror(prog, ...) gds_logerror_line((prog), \
        __FILE__, __LINE__, true, GDS_ERROR_NOQUIT, __VA_ARGS__)

/*!
 * \brief           Prints an error message.
 * \ingroup         general
 * \param prog      The program name to include in the error message.
 * \param ...       Other arguments, the first of which should be a format
 * string suitable for passing to `vprintf()`, optionally followed by any
 * additional arguments specified by the format string.
 */
#define log_error(prog, ...) gds_logerror_line((prog), \
        __FILE__, __LINE__, false, GDS_ERROR_NOQUIT, __VA_ARGS__)

/*!
 * \brief           Prints an error message with error number and exits.
 * \ingroup         general
 * \details         This macro can be called to print an error message
 * and quit following a function which has indicated failure and has set
 * `errno`. A message containing the error number and a text representation
 * of that error will be printed, following by the message supplied to
 * the function. This function is intended to be called from the corresponding
 * macro.
 * \param prog      The program name to include in the error message.
 * \param ...       Other arguments, the first of which should be a format
 * string suitable for passing to `vprintf()`, optionally followed by any
 * additional arguments specified by the format string.
 */
#define quit_strerror(prog, ...) gds_logerror_line((prog), \
        __FILE__, __LINE__, true, GDS_ERROR_EXIT, __VA_ARGS__)

/*!
 * \brief           Prints an error message and exits.
 * \ingroup         general
 * \param prog      The program name to include in the error message.
 * \param ...       Other arguments, the first of which should be a format
 * string suitable for passing to `vprintf()`, optionally followed by any
 * additional arguments specified by the format string.
 */
#define quit_error(prog, ...) gds_logerror_line((prog), \
        __FILE__, __LINE__, false, GDS_ERROR_EXIT, __VA_ARGS__)

/*!
 * \brief           Prints an error message and aborts.
 * \ingroup         general
 * \param prog      The program name to include in the error message.
 * \param ...       Other arguments, the first of which should be a format
 * string suitable for passing to `vprintf()`, optionally followed by any
 * additional arguments specified by the format string.
 */
#define abort_error(prog, ...) gds_logerror_line((prog), \
        __FILE__, __LINE__, false, GDS_ERROR_ABORT, __VA_ARGS__)

/*!
 * \brief           Tests an assertion and aborts on failure.
 * \ingroup         general
 * \param cond      The assertion to test.
 * \param prog      The program name to include in the error message.
 * \param ...       Other arguments, the first of which should be a format
 * string suitable for passing to `vprintf()`, optionally followed by any
 * additional arguments specified by the format string.
 */
#ifndef NDEBUG
#define gds_assert(cond, prog, ...) if ( !(cond) ) \
    gds_logerror_line((prog), __FILE__, __LINE__, \
           false, GDS_ERROR_ASSERT, __VA_ARGS__)
#else
#define gds_assert(cond, prog, ...)
#endif

/*!
 * \brief           Logs an error message.
 * \details         This function is intended to be called via the
 * accompanying macros.
 * \ingroup         general
 * \param progname  The program name to include in the message.
 * \param filename  The name of the source file.
 * \param linenum   The line number of the source file.
 * \param log_errno Set to `true` to include the current value of `errno`
 * and the string representation of that error in the message.
 * \param quit_type Info on how to quit the function.
 * \param fmt       The format string for the message to print. Format
 * specifiers are the same as the `printf()` family of functions.
 * \param ...       Any arguments to the format string.
 */
void gds_logerror_line(const char * progname,
                       const char * filename,
                       const int linenum,
                       const bool log_errno,
                       const enum gds_error_quit_type quit_type,
                       const char * fmt, ...);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_ERROR_H  */
