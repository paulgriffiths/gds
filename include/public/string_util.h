/*!
 * \file            string_util.h
 * \brief           Interface to string utility functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_STRING_UTIL_H
#define PG_GENERIC_DATA_STRUCTURES_STRING_UTIL_H

/*!
 * \brief           Structure to hold a string pair.
 */
struct pair_string {
    char * first;           /*!<  First string of pair      */
    char * second;          /*!<  Second string of pair     */
};

/*!
 * \brief           Trims CR and LF characters from the end of a string.
 * \ingroup         string_util
 * \param str       The string to trim.
 * \returns         A pointer to the passed string.
 */
char * trim_line_ending(char * str);

/*!
 * \brief           Trims trailing whitespace from a string.
 * \ingroup         string_util
 * \param str       The string to trim.
 * \returns         A pointer to the passed string.
 */
char * trim_right(char * str);

/*!
 * \brief           Trims leading whitespace from a string.
 * \ingroup         string_util
 * \param str       The string to trim.
 * \returns         A pointer to the passed string.
 */
char * trim_left(char * str);

/*!
 * \brief           Trims leading and trailing whitespace from a string
 * \ingroup         string_util
 * \param str       The string to trim.
 * \returns         A pointer to the passed string.
 */
char * trim(char * str);

/*!
 * \brief           Duplicates a string.
 * \ingroup         string_util
 * \param str       The string to duplicate.
 * \retval NULL     Failure, dynamic memory allocation failed
 * \retval non-NULL A pointer to the duplicated string
 */
char * pg_strdup(const char * str);

/*!
 * \brief           Duplicates at most n characters of a string.
 * \ingroup         string_util
 * \param str       The string to duplicate.
 * \param n         The maximum number of characters to duplicate.
 * \retval NULL     Failure, dynamic memory allocation failed
 * \retval non-NULL A pointer to the duplicated string
 */
char * pg_strndup(const char * str, const size_t n);

/*!
 * \brief           Splits a string into a string pair.
 * \ingroup         string_util
 * \param str       The string to split.
 * \param delim     The character on which to split.
 * \retval NULL     Failure, dynamic memory allocation failed
 * \retval non-NULL A pointer to the new string pair
 */
struct pair_string * pair_string_create(const char * str, const char delim);

/*!
 * \brief           Destroys a string pair.
 * \ingroup         string_util
 * \param pair      The pair to destroy.
 */
void pair_string_destroy(struct pair_string * pair);

#endif      /*  PGUTILS_GENERIC_DATA_STRUCTURES_STRING_UTIL_H  */
