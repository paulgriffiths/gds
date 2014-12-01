/*!
 * \file            gds_opt.h
 * \brief           Interface to command line option functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GDS_OPT_H
#define PG_GENERIC_DATA_STRUCTURES_GDS_OPT_H

#include <stdbool.h>

/*!
 * \brief               Parses a command line for options and non-options.
 * \ingroup cmdline
 * \param allowed       A string containing the allowed options. Each option
 * should be specified by a single alphabetic character. A ':' after an
 * option signifies that it can take an argument.
 * \param argv          List of command line strings passed to `main()`.
 * \retval true         Success
 * \retval false        Failure, due to memory allocation failure, or
 * badly-specified allowed options string, or unrecognized options
 */
bool gds_parse_options(const char * allowed, char ** argv);

/*!
 * \brief               Frees memory associated with command line options.
 * \ingroup cmdline
 */
void gds_free_options(void);

/*!
 * \brief               Returns the program name.
 * \details             The string returned is equivalent to `argv[0]`.
 * \ingroup cmdline
 * \retval non-NULL     The program name
 * \retval NULL         Error, or command line has not yet been parsed,
 * or program name was not present.
 */
const char * gds_option_progname(void);

/*!
 * \brief               Checks if an option was provided on the command line.
 * \ingroup cmdline
 * \param optname       A string containing the single-character option name.
 * \retval true         Option was provided
 * \retval false        Option was not provided, or command line has not
 * yet been parsed.
 */
bool gds_option_present(const char * optname);

/*!
 * \brief               Retrieves a string argument for an option.
 * \ingroup cmdline
 * \param optname       A string containing the single-character option name.
 * \retval non-NULL     The string argument
 * \retval NULL         Failure, argument was not provided, or command
 * line has not yet been parsed.
 */
const char * gds_option_argument_string(const char * optname);

/*!
 * \brief               Retrieves an integer argument for an option.
 * \ingroup cmdline
 * \param optname       A string containing the single-character option name.
 * \param value         A pointer to an `int` in which to store the value
 * of the argument. If this is `NULL`, the function merely checks whether
 * an argument representable by an integer is present.
 * \retval non-NULL     The string argument
 * \retval NULL         Failure, argument was not provided, argument could
 * not be represented as an integer, or command line has not yet been parsed.
 */
bool gds_option_argument_int(const char * optname, int * value);

/*!
 * \brief               Returns the number of non-option arguments provided.
 * \details             A non-option argument is any command line argument
 * not of the form `-x`, where `x` is any alphanumeric character.
 * \ingroup cmdline
 * \returns             The number of non-option arguments provided. Zero
 * is returned if the command line has not yet been parsed.
 */
int gds_option_nonopts_number(void);

/*!
 * \brief               Retrieves a non-option argument.
 * \ingroup cmdline
 * \param index         The zero-based index of the non-option argument.
 * \returns non-NULL    The non-option argument at the specified index
 * \returns NULL        Index out-of-range, or command line has not yet
 * been parsed
 */
const char * gds_option_nonopt(const size_t index);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GDS_OPT_H  */
