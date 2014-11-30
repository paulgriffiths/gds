/*!
 * \file            gds_util_std_wrappers.h
 * \brief           Interface to wrappers for standard functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_STD_WRAPPERS_H
#define PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_STD_WRAPPERS_H

#define xmalloc(s) gds_xmalloc((s), __FILE__, __LINE__)
#define xcalloc(n, s) gds_xcalloc((n), (s), __FILE__, __LINE__)
#define xrealloc(p, s) gds_xrealloc((p), (s), __FILE__, __LINE__)
#define xstrdup(str) gds_xstrdup((str), __FILE__, __LINE__)

void * gds_xmalloc(const size_t size,
                   const char * filename, const int linenum);
void * gds_xcalloc(const size_t nmemb, const size_t size,
                   const char * filename, const int linenum);
void * gds_xrealloc(void * ptr, const size_t size,
                    const char * filename, const int linenum);
char * gds_xstrdup(const char * str,
                   const char * filename, const int linenum);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_STD_WRAPPERS_H  */
