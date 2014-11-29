/*!
 * \file            gds_util_error.h
 * \brief           Interface to general utility error functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_ERROR_H
#define PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_ERROR_H

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
#define quit_strerror(prog, ...) gds_strerror_line_quit(prog, \
        __FILE__, __LINE__, __VA_ARGS__)

/*!
 * \brief           Prints an error message and exits.
 * \ingroup         general
 * \param prog      The program name to include in the error message.
 * \param ...       Other arguments, the first of which should be a format
 * string suitable for passing to `vprintf()`, optionally followed by any
 * additional arguments specified by the format string.
 */
#define quit_error(prog, ...) gds_error_line_quit(prog, \
        __FILE__, __LINE__, __VA_ARGS__)

/*!
 * \brief           Prints an error message with error number and exits.
 * \ingroup         general
 * \details         This function can be called to print an error message
 * and quit following a function which has indicated failure and has set
 * `errno`. A message containing the error number and a text representation
 * of that error will be printed, following by the message supplied to
 * the function. This function is intended to be called from the corresponding
 * macro.
 * \param progname  The program name to include in the message.
 * \param filename  The name of the source file.
 * \param linenum   The line number of the source file.
 * \param fmt       The format string for the message to print. Format
 * specifiers are the same as the `printf()` family of functions.
 * \param ...       Any arguments to the format string.
 */
void gds_strerror_line_quit(const char * progname,
                            const char * filename,
                            const int linenum,
                            const char * fmt, ...);

/*!
 * \brief           Prints an error message and exits.
 * \ingroup         general
 * \details         This function is intended to be called from the
 * corresponding macro.
 * \param progname  The program name to include in the message.
 * \param filename  The name of the source file.
 * \param linenum   The line number of the source file.
 * \param fmt       The format string for the message to print. Format
 * specifiers are the same as the `printf()` family of functions.
 * \param ...       Any arguments to the format string.
 */
void gds_error_line_quit(const char * progname,
                         const char * filename,
                         const int linenum,
                         const char * fmt, ...);

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

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_ERROR_H  */
