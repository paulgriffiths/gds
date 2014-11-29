/*!
 * \file            gds_string.c
 * \brief           Implementation of string data structure.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <assert.h>

#include "gds_string.h"

/*!  Structure to contain string  */
struct GDSString {
    char * data;        /*!<  The data in C-style string format     */
    size_t length;      /*!<  The length of the string              */
    size_t capacity;    /*!<  The size of the `data` buffer         */
};

/*!
 * \brief           Directly assigns dynamically allocated data to a string.
 * \param dst       The string to which to assign.
 * \param src       The dynamically allocated C-style string to assign.
 * \param size      The size of the allocated memory.
 * \param length    The length of the C-style string.
 * \returns         `dst`.
 */
static GDSString gds_str_assign_cstr_direct(GDSString dst,
                                            char * src,
                                            const size_t size,
                                            const size_t length);

/*!
 * \brief           Assigns a C-style string to a string with length.
 * \details         Providing the length avoids a call to `strlen()`, which
 * is more efficient if the length is already known.
 * \param dst       The string to which to assign.
 * \param src       The C-style string to be assigned.
 * \param length    The length of `src`, excluding the terminating null.
 * \returns         `dst` on success, `NULL` on failure.
 */
static GDSString gds_str_assign_cstr_length(GDSString dst,
                                            const char * src,
                                            const size_t length);

/*!
 * \brief           Duplicates a C-style string.
 * \details         This can be used in place of POSIX's `strdup()`.
 * \param src       The string to duplicate.
 * \param length    A pointer to a `size_t` variable to contain the length
 * of the duplicated string. This is provided for efficiency purposes, as
 * the length of the string needs to be calculated to duplicate it, so
 * modifying this parameter may help to avoid a second unnecessary call to
 * `strlen()`. This argument is ignored if set to `NULL`.
 * \returns         A pointer to the duplicated string, or `NULL` on failure.
 * The caller is responsible for `free()`ing this string.
 */
static char * duplicate_cstr(const char * src, size_t * length);

/*!
 * \brief               Changes the capacity of a string.
 * \param str           The string.
 * \param new_capacity  The new capacity.
 * \returns             `true` if the capacity was successfully changed,
 * `false` otherwise.
 */
static bool change_capacity(GDSString str, const size_t new_capacity);

/*!
 * \brief                   Changes the capacity of a string if needed.
 * \details                 If the string's existing capacity exceeds the
 * requirement capacity, it remains unchanged. Otherwise, the strings capacity
 * is increased to the required capacity.
 * \param str               The string.
 * \param required_capacity The required capacity.
 * \returns                 `true` if the capacity was successfully changed,
 * or if no change was needed, `false` if a capacity change was needed but
 * was not successful.
 */
static bool change_capacity_if_needed(GDSString str,
                                      const size_t required_capacity);

/*!
 * \brief           Truncates a string if necessary.
 * \details         This function truncates the length of a string, and
 * adds a terminating null character in the last place, if the string's
 * capacity is not sufficient to contain the string's current length.
 * This function would normally be called after a reduction in the capacity
 * of the string.
 * \param str       The string.
 */
static void truncate_if_needed(GDSString str);

/*!
 * \brief           Concatenates a C-style string to a string, with length.
 * \details         Passing the length avoids the need to call `strlen()`,
 * which is more efficient when we already know the length.
 * \param dst       The destination string.
 * \param src       The C-style string to concentate with `dst`.
 * \param src_length  The length of `src`, not including the terminating null.
 * \returns         `dst` on success, `NULL` on failure.
 */
static GDSString gds_str_concat_cstr_size(GDSString dst,
                                          const char * src,
                                          const size_t src_length);

/*!
 * \brief           Removes characters at the start of a string, in place.
 * \param str       The string.
 * \param numchars  The number of characters to remove.
 */
static void gds_str_remove_left(GDSString str, const size_t numchars);

/*!
 * \brief           Removes characters at the end of a string, in place.
 * \param str       The string.
 * \param numchars  The number of characters to remove.
 */
static void gds_str_remove_right(GDSString str, const size_t numchars);

GDSString gds_str_create_direct(char * init_str, const size_t init_str_size)
{
    assert(init_str && init_str_size > 0);

    GDSString new_str = malloc(sizeof *new_str);
    if ( !new_str ) {
        free(init_str);
        return NULL;
    }

    new_str->data = init_str;
    new_str->capacity = init_str_size;
    new_str->length = init_str_size - 1;

    return new_str;
}

GDSString gds_str_create(const char * init_str)
{
    size_t length;
    char * new_data = duplicate_cstr(init_str, &length);
    if ( !new_data ) {
        return NULL;
    }
    return gds_str_create_direct(new_data, length + 1);
}

GDSString gds_str_dup(GDSString src)
{
    return gds_str_create(src->data);
}

GDSString gds_str_create_sprintf(const char * format, ...)
{
    static char dummy_buffer[1];

    /*  Determine amount of memory needed  */

    va_list ap;
    va_start(ap, format);
    size_t num_written = vsnprintf(dummy_buffer, 1, format, ap);
    va_end(ap);

    /*  Allocate correct amount of memory  */

    const size_t required_alloc = num_written + 1;
    char * new_data = malloc(required_alloc);
    if ( !new_data ) {
        return NULL;
    }

    /*  Write formatted string  */

    va_start(ap, format);
    num_written = vsnprintf(new_data, required_alloc, format, ap);
    va_end(ap);

    /*  Create and return new string  */

    assert((num_written + 1) == required_alloc);
    return gds_str_create_direct(new_data, required_alloc);
}

void gds_str_destroy(GDSString str)
{
    if ( str ) {

        /*  Debug sanity checks  */
        assert(strlen(str->data) == str->length);
        assert(str->capacity > str->length);

        free(str->data);
        free(str);
    }
}

void gds_str_destructor(void * str)
{
    gds_str_destroy(str);
}

GDSString gds_str_assign(GDSString dst, GDSString src)
{
    return gds_str_assign_cstr_length(dst, src->data, src->length);
}

GDSString gds_str_assign_cstr(GDSString dst, const char * src)
{
    return gds_str_assign_cstr_length(dst, src, strlen(src));
}

const char * gds_str_cstr(GDSString str)
{
    return str->data;
}

size_t gds_str_length(GDSString str)
{
    return str->length;
}

GDSString gds_str_size_to_fit(GDSString str)
{
    const size_t max_capacity = str->length + 1;
    if ( str->capacity > max_capacity ) {
        if ( !change_capacity(str, max_capacity) ) {
            return NULL;
        }
    }

    return str;
}

GDSString gds_str_concat(GDSString dst, GDSString src)
{
    return gds_str_concat_cstr_size(dst, src->data, src->length);
}

GDSString gds_str_concat_cstr(GDSString dst, const char * src)
{
    return gds_str_concat_cstr_size(dst, src, strlen(src));
}

GDSString gds_str_trunc(GDSString str, const size_t length)
{
    const size_t new_capacity = length + 1;
    if ( new_capacity < str->capacity ) {
        if ( !change_capacity(str, new_capacity) ) {
            return NULL;
        }
    }

    return str;
}

unsigned long gds_str_hash(GDSString str)
{
    unsigned long hash = 5381;
    int c;
    const char * c_str = str->data;

    while ( (c = *c_str++) ) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int gds_str_compare(GDSString s1, GDSString s2)
{
    const int r = strcmp(gds_str_cstr(s1), gds_str_cstr(s2));
    if ( r < 0 ) {
        return -1;
    }
    else if ( r > 0 ) {
        return 1;
    }
    else {
        return 0;
    }
}

int gds_str_compare_cstr(GDSString s1, const char * s2)
{
    const int r = strcmp(gds_str_cstr(s1), s2);
    if ( r < 0 ) {
        return -1;
    }
    else if ( r > 0 ) {
        return 1;
    }
    else {
        return 0;
    }
}

int gds_str_strchr(GDSString str, const char ch, const int start)
{
    assert(start < (int) str->length);

    int i = start;
    while ( str->data[i] && str->data[i] != ch ) {
        ++i;
    }
    return str->data[i] ? i : -1;
}

GDSString gds_str_substr_left(GDSString str, const size_t numchars)
{
    GDSString new_substr = gds_str_dup(str);
    gds_str_remove_right(new_substr, new_substr->length - numchars);
    return new_substr;
}

GDSString gds_str_substr_right(GDSString str, const size_t numchars)
{
    GDSString new_substr = gds_str_dup(str);
    gds_str_remove_left(new_substr, new_substr->length - numchars);
    return new_substr;
}

void gds_str_split(GDSString src, GDSString * left,
                   GDSString * right, const char sc)
{
    int idx = gds_str_length(src) ? gds_str_strchr(src, sc, 0) : -1;
    if ( idx == -1 ) {
        *left = gds_str_dup(src);
        *right = NULL;
    }
    else {
        *left = gds_str_substr_left(src, idx);
        *right = gds_str_substr_right(src, src->length - idx - 1);
    }
}
 
void gds_str_trim_leading(GDSString str)
{
    size_t i = 0;
    while ( str->data[i] && isspace(str->data[i]) ) {
        ++i;
    }
    gds_str_remove_left(str, i);
}

void gds_str_trim_trailing(GDSString str)
{
    assert(str);

    int i = str->length - 1;
    size_t num = 0;

    while ( i >= 0 && isspace(str->data[i]) ) {
        --i;
        ++num;
    }
    gds_str_remove_right(str, num);
}

void gds_str_trim(GDSString str)
{
    gds_str_trim_trailing(str);
    gds_str_trim_leading(str);
}

char gds_str_char_at_index(GDSString str, const size_t index)
{
    assert(index < str->length);
    return str->data[index];
}

bool gds_str_is_empty(GDSString str)
{
    return str->length == 0;
}

bool gds_str_is_alnum(GDSString str)
{
    if ( gds_str_is_empty(str) ) {
        return false;
    }

    bool result = true;
    for ( size_t i = 0; i < str->length; ++i ) {
        if ( !isalnum(str->data[i]) ) {
            result = false;
            break;
        }
    }
    return result;
}

void gds_str_clear(GDSString str)
{
    str->data[0] = '\0';
    str->length = 0;
}

bool gds_str_intval(GDSString str, const int base, int * value)
{
    char * endptr;
    const long n = strtol(str->data, &endptr, base);
    if ( *endptr ) {
        if ( value ) {
            *value = 0;
        }
        return false;
    }
    else {
        if ( value ) {
            *value = (int) n;
        }
        return true;
    }
}

bool gds_str_doubleval(GDSString str, double * value)
{
    char * endptr;
    const double n = strtod(str->data, &endptr);
    if ( *endptr ) {
        if ( value ) {
            *value = 0;
        }
        return false;
    }
    else {
        if ( value ) {
            *value = n;
        }
        return true;
    }
}

GDSString gds_str_getline(GDSString str, const size_t size, FILE * fp)
{
    char * buffer = malloc(size);
    if ( !buffer ) {
        return NULL;
    }

    if ( !fgets(buffer, size, fp) ) {
        free(buffer);
        return NULL;
    }

    size_t length = strlen(buffer);
    if ( length && buffer[length - 1] == '\n' ) {
        buffer[length - 1] = '\0';
        --length;
    }
    return gds_str_assign_cstr_direct(str, buffer, size, length);
}

GDSString gds_str_decorate(GDSString str,
                           GDSString left_dec, GDSString right_dec)
{
    GDSString result;
    if ( right_dec ) {
        result = gds_str_create_sprintf("%s%s%s",
                                       gds_str_cstr(left_dec),
                                       gds_str_cstr(str),
                                       gds_str_cstr(right_dec));
    }
    else {
        result = gds_str_create_sprintf("%s%s%s",
                                       gds_str_cstr(left_dec),
                                       gds_str_cstr(str),
                                       gds_str_cstr(left_dec));
    }
    return result;
}

static GDSString gds_str_assign_cstr_direct(GDSString dst,
                                            char * src,
                                            const size_t size,
                                            const size_t length)
{
    assert(size > 0 && length < size);

    free(dst->data);
    dst->data = src;
    dst->capacity = size;
    dst->length = length;

    return dst;
}

static GDSString gds_str_assign_cstr_length(GDSString dst,
                                            const char * src,
                                            const size_t length)
{
    const size_t req_cap = length + 1;
    if ( !change_capacity_if_needed(dst, req_cap) ) {
        return NULL;
    }

    memcpy(dst->data, src, req_cap);
    dst->length = length;
    return dst;
}

static char * duplicate_cstr(const char * src, size_t * length)
{
    const size_t src_length = strlen(src);
    char * new_str = malloc(src_length + 1);
    if ( !new_str ) {
        return NULL;
    }

    memcpy(new_str, src, src_length + 1);
    if ( length ) {
        *length = src_length;
    }

    return new_str;
}

static bool change_capacity(GDSString str, const size_t new_capacity)
{
    assert(new_capacity > 0);

    bool did_reallocate = true;

    char * temp = realloc(str->data, new_capacity);
    if ( temp ) {
        str->data = temp;
        str->capacity = new_capacity;
        truncate_if_needed(str);
    }
    else {
        did_reallocate = false;
    }

    return did_reallocate;
}

static bool change_capacity_if_needed(GDSString str,
                                      const size_t required_capacity)
{
    assert(required_capacity > 0);

    if ( required_capacity > str->capacity ) {
        return change_capacity(str, required_capacity);
    }

    return true;
}

static void truncate_if_needed(GDSString str)
{
    if ( str->length >= str->capacity ) {
        str->length = str->capacity - 1;
        str->data[str->length] = '\0';
    }
}

static GDSString gds_str_concat_cstr_size(GDSString dst,
                                          const char * src,
                                          const size_t src_length)
{
    const size_t req_cap = dst->length + src_length + 1;
    if ( !change_capacity_if_needed(dst, req_cap) ) {
        return NULL;
    }
    memcpy(dst->data + dst->length, src, src_length + 1);
    dst->length += src_length;
    return dst;
}

static void gds_str_remove_right(GDSString str, const size_t numchars)
{
    if ( numchars > 0 && numchars <= str->length ) {
        const size_t remaining = str->length - numchars;
        str->data[remaining] = '\0';
        str->length -= numchars;
    }
}

static void gds_str_remove_left(GDSString str, const size_t numchars)
{
    if ( numchars > 0 && numchars <= str->length ) {
        const size_t remaining = str->length - numchars;
        memmove(str->data, str->data + numchars, remaining + 1);
        str->length -= numchars;
    }
}
