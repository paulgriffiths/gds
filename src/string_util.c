/*!
 * \file            string_util.c
 * \brief           Implementation of string utility functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "string_util.h"

/*!
 * \brief           Helper function to resize a string list.
 * \param list      The string list to resize.
 * \param capacity  The new capacity.
 * \retval false    Failure, dynamic memory reallocation failed.
 * \retval true     Success.
 */
static bool list_string_resize(struct list_string * list,
                               const size_t capacity);

char * gds_trim_line_ending(char * str)
{
    if ( *str != '\0' ) {
        for ( int last = (int) strlen(str) - 1;
              last >= 0 && ( str[last] == '\n' || str[last] == '\r' );
              --last ) {
            str[last] = 0;
        }
    }

    return str;
}

char * gds_trim_right(char * str)
{
    if ( *str != '\0' ) {
        for ( int last = (int) strlen(str) - 1;
              last >= 0 && isspace(str[last]);
              --last ) {
            str[last] = 0;
        }
    }

    return str;
}

char * gds_trim_left(char * str)
{
    char * leading = str;
    char * orig_head = str;

    while ( *leading != '\0' && isspace(*leading) ) {
        ++leading;
    }

    do {
        *orig_head++ = *leading;
    } while ( *leading++ );

    return str;
}

char * gds_trim(char * str)
{
    gds_trim_left(str);
    gds_trim_right(str);
    return str;
}

char * gds_strdup(const char * str)
{
    return gds_strndup(str, strlen(str));
}

char * gds_strndup(const char * str, const size_t n)
{
    const size_t len = strlen(str);
    const size_t ncopy = len > n ? n : len;

    char * new_str = malloc(ncopy + 1);
    if ( !new_str ) {
        return NULL;
    }

    strncpy(new_str, str, ncopy);
    new_str[ncopy] = 0;

    return new_str;
}

struct pair_string * pair_string_create(const char * str, const char delim)
{
    struct pair_string * new_pair = malloc(sizeof *new_pair);
    if ( !new_pair ) {
        return NULL;
    }

    const char * needle = strchr(str, delim);
    if ( !needle || delim == 0 ) {
        if ( !(new_pair->first = gds_strdup(str)) ) {
            free(new_pair);
            return NULL;
        }

        if ( !(new_pair->second = gds_strdup("")) ) {
            free(new_pair->first);
            return NULL;
        }
    }
    else {
        const size_t len_first = needle - str;
        const size_t len_second = strlen(str) - len_first - 1;

        if ( !(new_pair->first = gds_strndup(str, len_first)) ) {
            free(new_pair);
            return NULL;
        }

        if ( !(new_pair->second = gds_strndup(needle + 1, len_second)) ) {
            free(new_pair->first);
            free(new_pair);
            return NULL;
        }
    }

    return new_pair;
}

struct pair_string * pair_string_copy(const struct pair_string * pair)
{
    struct pair_string * new_pair = malloc(sizeof *new_pair);
    if ( !new_pair ) {
        return NULL;
    }

    new_pair->first = gds_strdup(pair->first);
    if ( !new_pair->first ) {
        free(new_pair);
        return NULL;
    }

    new_pair->second = gds_strdup(pair->second);
    if ( !new_pair->second ) {
        free(new_pair->first);
        free(new_pair);
        return NULL;
    }

    return new_pair;
}

void pair_string_destroy(struct pair_string * pair)
{
    free(pair->first);
    free(pair->second);
    free(pair);
}

struct list_string * list_string_create(const size_t n)
{
    struct list_string * new_list = malloc(sizeof *new_list);
    if ( !new_list ) {
        return NULL;
    }

    new_list->list = malloc(n * sizeof *new_list->list);
    if ( !new_list->list ) {
        free(new_list);
        return NULL;
    }

    new_list->size = 0;
    new_list->list[0] = NULL;

    return new_list;
}

void list_string_destroy(struct list_string * list)
{
    size_t i = 0;
    while ( list->list[i] && i < list->size ) {
        free(list->list[i++]);
    }
    free(list->list);
    free(list);
}

static bool list_string_resize(struct list_string * list, const size_t capacity)
{
    char ** temp_list = realloc(list->list, capacity * sizeof *temp_list);
    if ( !temp_list ) {
        return false;
    }
    else {
        list->list = temp_list;
        return true;
    }
}

struct list_string * split_string(const char * str, const char delim)
{
    size_t list_capacity = 10;
    struct list_string * new_list = list_string_create(list_capacity);
    if ( !new_list ) {
        return NULL;
    }

    const char * p = str;
    const char * tok = str;
    
    while ( *p && tok ) {
        if ( new_list->size == list_capacity - 1 ) {

            /*  Resize list if necessary  */

            list_capacity *= 2;
            if ( !list_string_resize(new_list, list_capacity) ) {
                list_string_destroy(new_list);
                return NULL;
            }
        }

        if ( (tok = strchr(p, delim)) ) {
            
            /*  Copy up to delimiter if found...  */

            const size_t len_tok = tok - p;
            new_list->list[new_list->size] = gds_strndup(p, len_tok);
            if ( !new_list->list[new_list->size] ) {
                list_string_destroy(new_list);
                return NULL;
            }
        }
        else {

            /*  ...or copy rest of string if not.  */

            new_list->list[new_list->size] = gds_strdup(p);
            if ( !new_list->list[new_list->size] ) {
                list_string_destroy(new_list);
                return NULL;
            }
        }

        new_list->size += 1;
        new_list->list[new_list->size] = NULL;

        if ( tok ) {

            /*  If we found the delimiter, increment p just past it  */

            p = tok + 1;
        }
    }

    /*  Downsize list if necessary to avoid wasting space  */

    if ( list_capacity != new_list->size + 1 ) {
        if ( !list_string_resize(new_list, new_list->size + 1) ) {
            list_string_destroy(new_list);
            return NULL;
        }
    }

    return new_list;
}
