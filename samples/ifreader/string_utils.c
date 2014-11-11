/*!
 * \file            string_utils.c
 * \brief           Implementation of string utility functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "string_utils.h"

char * trim_line_ending(char * str)
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

char * trim_right(char * str)
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

char * trim_left(char * str)
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

char * trim(char * str)
{
    trim_left(str);
    trim_right(str);
    return str;
}

char * pg_strdup(const char * str)
{
    return pg_strndup(str, strlen(str));
}

char * pg_strndup(const char * str, const size_t n)
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
        if ( !(new_pair->first = pg_strdup(str)) ) {
            free(new_pair);
            return NULL;
        }

        if ( !(new_pair->second = pg_strdup("")) ) {
            free(new_pair->first);
            return NULL;
        }
    }
    else {
        const size_t len_first = needle - str;
        const size_t len_second = strlen(str) - len_first - 1;

        if ( !(new_pair->first = pg_strndup(str, len_first)) ) {
            free(new_pair);
            return NULL;
        }

        if ( !(new_pair->second = pg_strndup(needle + 1, len_second)) ) {
            free(new_pair->first);
            free(new_pair);
            return NULL;
        }
    }

    return new_pair;
}

void pair_string_destroy(struct pair_string * pair)
{
    free(pair->first);
    free(pair->second);
    free(pair);
}
