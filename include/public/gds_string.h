/*!
 * \file            gds_string.h
 * \brief           Interface to string data structure.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_STRING_H
#define PG_GENERIC_DATA_STRUCTURES_STRING_H

#include <stdio.h>
#include <stdbool.h>

/*!
 * \brief           Opaque data type for string.
 * \ingroup         gds_string
 */
typedef struct GDSString * GDSString;

/*!
 * \brief           Creates a new string from a C-style string.
 * \ingroup         gds_string
 * \param init_str  The C-style string.
 * \returns         The new string, or `NULL` on failure.
 */
GDSString gds_str_create(const char * init_str);

/*!
 * \brief           Creates a new string from another string.
 * \ingroup         gds_string
 * \param src       The other string.
 * \returns         The new string, or `NULL` on failure.
 */
GDSString gds_str_dup(GDSString src);

/*!
 * \brief           Creates a string with `sprintf()`-type format.
 * \ingroup         gds_string
 * \param format    The format string.
 * \param ...       The subsequent arguments as specified by the format string.
 * \returns         The new string, or `NULL` on failure.
 */
GDSString gds_str_create_sprintf(const char * format, ...);

/*!
 * \brief               Creates a string using allocated memory.
 * \details             The normal construction functions duplicate the
 * string used to create it. In cases where allocated memory is already
 * available (e.g. in `gds_str_create_sprintf()`) this function allows
 * that memory to be directly assigned to the string, avoiding an
 * unnecessary duplication.
 * \ingroup         gds_string
 * \param init_str      The allocated memory. IMPORTANT: If the construction
 * of the string fails, this memory will be `free()`d.
 * \param init_str_size The size of the allocated memory. IMPORTANT: The
 * string's length is assumed to be one less than this quantity, and a
 * call to `strlen()` is NOT performed.
 * \returns             The new string, or `NULL` on failure.
 */
GDSString gds_str_create_direct(char * init_str, const size_t init_str_size);

/*!
 * \brief           Destroys a string and releases allocated resources.
 * \ingroup         gds_string
 * \param str       The string to destroy..
 */
void gds_str_destroy(GDSString str);

/*!
 * \brief           Destroys a string and releases allocated resources.
 * \details         This function calls `gds_str_destroy()`, and can be passed
 * \ingroup         gds_string
 * to a data structure expecting a destructor function with the signature
 * void (*)(void *).
 * \param str       The string to destroy.
 */
void GDSString_destructor(void * str);

/*!
 * \brief           Assigns a string to another.
 * \ingroup         gds_string
 * \param dst       The destination string.
 * \param src       The source string.
 * \returns         `dst` on success, `NULL` on failure.
 */
GDSString gds_str_assign(GDSString dst, GDSString src);

/*!
 * \brief           Assigns a C-style string to a string.
 * \ingroup         gds_string
 * \param dst       The destination string.
 * \param src       The source C-style string.
 * \returns         `dst` on success, `NULL` on failure.
 */
GDSString gds_str_assign_cstr(GDSString dst, const char * src);

/*!
 * \brief           Returns a C-style string containing the string's contents.
 * \ingroup         gds_string
 * \param str       The string.
 * \returns         The C-style string containing the string's contents. The
 * caller should not directly modify this string.
 */
const char * gds_str_cstr(GDSString str);

/*!
 * \brief           Returns the length of a string.
 * \ingroup         gds_string
 * \param str       The string.
 * \returns         The length of the string.
 */
size_t gds_str_length(GDSString str);

/*!
 * \brief           Reduces a string's capacity to fit its length.
 * \ingroup         gds_string
 * \param str       The string to size.
 * \returns         `str`, or `NULL` on failure.
 */ 
GDSString gds_str_size_to_fit(GDSString str);

/*!
 * \brief           Concatenates two strings.
 * \ingroup         gds_string
 * \param dst       The destination string.
 * \param src       The source strings.
 * \returns         The destination string, or `NULL` on failure.
 */
GDSString gds_str_concat(GDSString dst, GDSString src);

/*!
 * \brief           Concatenates a C-style string to a string.
 * \ingroup         gds_string
 * \param dst       The destination string.
 * \param src       The source strings.
 * \returns         The destination string, or `NULL` on failure.
 */
GDSString gds_str_concat_cstr(GDSString dst, const char * src);

/*!
 * \brief           Truncates a string.
 * \ingroup         gds_string
 * \param str       The string.
 * \param length    The new length to which to truncate.
 * \returns         The original string, or `NULL` on failure.
 */
GDSString gds_str_trunc(GDSString str, const size_t length);

/*!
 * \brief           Calculates a hash of a string.
 * \details         Uses Dan Bernstein's djb2 algorithm.
 * \ingroup         gds_string
 * \param str       The string.
 * \returns         The hash value
 */
unsigned long gds_str_hash(GDSString str);

/*!
 * \brief           Compares two strings.
 * \ingroup         gds_string
 * \param s1        The first string.
 * \param s2        The second string.
 * \returns         Less than, equal to, or greater than zero if s1 is found,
 * respectively, to be less than, equal to, or greater than s2.
 */
int gds_str_compare(GDSString s1, GDSString s2);

/*!
 * \brief           Compares a string with a C-style string.
 * \ingroup         gds_string
 * \param s1        The first string.
 * \param s2        The second, C-Style string.
 * \returns         Less than, equal to, or greater than zero if s1 is found,
 * respectively, to be less than, equal to, or greater than s2.
 */
int gds_str_compare_cstr(GDSString s1, const char * s2);

/*!
 * \brief           Returns index of first occurence of a character.
 * \ingroup         gds_string
 * \param str       The string.
 * \param ch        The character for which to search.
 * \param start     The index of the string at which to start looking. Set
 * this to non-zero to begin searching from a point other than the first
 * character of the string.
 * \returns         The index of the first occurence, or -1 if the character
 * was not found.
 */
int gds_str_strchr(GDSString str, const char ch, const int start);

/*!
 * \brief           Returns a left substring.
 * \ingroup         gds_string
 * \param str       The string.
 * \param numchars  The number of left characters to return. If this is
 * greater than the length of the string, the whole string is returned.
 * \returns         A new string representing the substring.
 */
GDSString gds_str_substr_left(GDSString str, const size_t numchars);

/*!
 * \brief           Returns a right substring.
 * \ingroup         gds_string
 * \param str       The string.
 * \param numchars  The number of right characters to return. If this is
 * greater than the length of the string, the whole string is returned.
 * \returns         A new string representing the substring.
 */
GDSString gds_str_substr_right(GDSString str, const size_t numchars);

/*!
 * \brief           Splits a string
 * \ingroup         gds_string
 * \param src       The string to split.
 * \param left      Pointer to left substring (modified)
 * \param right     Pointer to right substring (modified)
 * \param sc        Split character.
 */
void gds_str_split(GDSString src, GDSString * left,
                   GDSString * right, const char sc);

/*!
 * \brief           Trims leading whitespace in-place.
 * \ingroup         gds_string
 * \param str       The string.
 */
void gds_str_trim_leading(GDSString str);

/*!
 * \brief           Trims trailing whitespace in-place.
 * \ingroup         gds_string
 * \param str       The string.
 */
void gds_str_trim_trailing(GDSString str);

/*!
 * \brief           Trims leading and trailing whitespace in-place.
 * \ingroup         gds_string
 * \param str       The string.
 */
void gds_str_trim(GDSString str);

/*!
 * \brief           Returns the character at a specified index.
 * \ingroup         gds_string
 * \param str       The string.
 * \param index     The specified index.
 * \returns         The character at the specified index.
 */
char gds_str_char_at_index(GDSString str, const size_t index);

/*!
 * \brief           Checks if a string is empty.
 * \ingroup         gds_string
 * \param str       The string.
 * \returns         `true` is the string is empty, `false` otherwise.
 */
bool gds_str_is_empty(GDSString str);

/*!
 * \brief           Checks is a string contains only alphanumeric characters.
 * \details         The string must contain *some* alphanumeric characters to
 * check `true`, i.e. the string must be non-empty. Thus it can be used to
 * check that a string does indeed contain content, and that that content
 * is solely alphanumeric.
 * \ingroup         gds_string
 * \param str       The string.
 * \returns         `true` if the string contains only alphanumeric characters,
 * `false` otherwise.
 */
bool gds_str_is_alnum(GDSString str);

/*!
 * \brief           Clears (empties) a string.
 * \ingroup         gds_string
 * \param str       The string.
 */
void gds_str_clear(GDSString str);

/*!
 * \brief           Gets the integer value of a string.
 * \ingroup         gds_string
 * \param str       The string.
 * \param base      The base of the integer. This has the same meaning as
 * the third argument to standard C `strtol()`.
 * \param value     A pointer to the integer in which to store the value.
 * Zero is stored if the string does not contain a valid integer value.
 * \returns         `true` on successful conversion, `false` if the string
 * does not contain a valid integer value.
 */
bool gds_str_intval(GDSString str, const int base, int * value);

/*!
 * \brief           Gets the double value of a string.
 * \ingroup         gds_string
 * \param str       The string.
 * \param value     A pointer to the double in which to store the value.
 * Zero is stored if the string does not contain a valid double value.
 * \returns         `true` on successful conversion, `false` if the string
 * does not contain a valid double value.
 */
bool gds_str_doubleval(GDSString str, double * value);

/*!
 * \brief           Gets a line from a file and assigns it to a string.
 * \details         Any trailing newline character is stripped.
 * \ingroup         gds_string
 * \param str       The string.
 * \param size      The maximum number of bytes to read, including the null.
 * \param fp        The file pointer from which to read.
 * \returns         `dst`
 */
GDSString gds_str_getline(GDSString str, const size_t size, FILE * fp);

/*!
 * \brief           Brackets a string with decoration strings.
 * \ingroup         gds_string
 * \param str       The string to decorate.
 * \param left_dec  The string to add to the left of `str`.
 * \param right_dec The string to add to the right of `str`, or `NULL`
 * to add `left_dec` to both sides.
 * \returns         The decorated string.
 */
GDSString gds_str_decorate(GDSString str,
                           GDSString left_dec, GDSString right_dec);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_STRING_H  */
