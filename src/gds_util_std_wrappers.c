/*!
 * \file            gds_util_std_wrappers.c
 * \brief           Implementation of wrappers for standard functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pggds/gds_util.h>

void * gds_xmalloc(const size_t size,
                   const char * filename, const int linenum)
{
    void * p = malloc(size);
    if ( !p ) {
        fprintf(gds_errlog(), "Memory allocation failed at %s:%d: %s (%d)\n",
                filename, linenum, strerror(errno), errno);
        abort();
    }
    return p;
}

void * gds_xcalloc(const size_t nmemb, const size_t size,
                   const char * filename, const int linenum)
{
    void * p = calloc(nmemb, size);
    if ( !p ) {
        fprintf(gds_errlog(), "Memory allocation failed at %s:%d: %s (%d)\n",
                filename, linenum, strerror(errno), errno);
        abort();
    }
    return p;
}

void * gds_xrealloc(void * ptr, const size_t size,
                   const char * filename, const int linenum)
{
    void * p = realloc(ptr, size);
    if ( !p ) {
        fprintf(gds_errlog(), "Memory reallocation failed at %s:%d: %s (%d)\n",
                filename, linenum, strerror(errno), errno);
        abort();
    }
    return p;
}

char * gds_xstrdup(const char * str,
                   const char * filename, const int linenum)
{
    char * new_str = gds_strdup(str);
    if ( !new_str ) {
        fprintf(gds_errlog(), "Memory allocation for strdup() failed at "
                "%s:%d: %s (%d)\n", filename, linenum,
                strerror(errno), errno);
        abort();
    }
    return new_str;
}

FILE * gds_xfopen(const char * path, const char * mode,
                  const char * filename, const int linenum)
{
    FILE * fp = fopen(path, mode);
    if ( !fp ) {
        fprintf(gds_errlog(), "Couldn't open file %s with mode \"%s\" at "
                "%s:%d: %s (%d)\n", path, mode, filename, linenum,
                strerror(errno), errno);
        exit(EXIT_FAILURE);
    }
    return fp;
}
