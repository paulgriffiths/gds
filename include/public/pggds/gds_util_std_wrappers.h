/*!
 * \file            gds_util_std_wrappers.h
 * \brief           Interface to wrappers for standard functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_STD_WRAPPERS_H
#define PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_STD_WRAPPERS_H

/*!
 * \brief           Macro to call malloc() and abort on failure.
 * \ingroup general
 * \param s         The number of bytes to allocate.
 */
#define xmalloc(s) gds_xmalloc((s), __FILE__, __LINE__)

/*!
 * \brief           Macro to call calloc() and abort on failure.
 * \ingroup general
 * \param n         The number of members to allocate.
 * \param s         The size in bytes of each member.
 */
#define xcalloc(n, s) gds_xcalloc((n), (s), __FILE__, __LINE__)

/*!
 * \brief           Macro to call realloc() and abort on failure.
 * \ingroup general
 * \param p         A pointer to the memory to reallocate.
 * \param s         The number of bytes in the new allocation.
 */
#define xrealloc(p, s) gds_xrealloc((p), (s), __FILE__, __LINE__)

/*!
 * \brief           Macro to call strdup() and abort on failure.
 * \ingroup general
 * \param str       The string to duplicate.
 */
#define xstrdup(str) gds_xstrdup((str), __FILE__, __LINE__)

/*!
 * \brief           Wraps malloc() and aborts on failure.
 * \details         This is designed to be called from the corresponding
 * macro.
 * \param size      The number of bytes to allocate.
 * \param filename  The name of the calling file.
 * \param linenum   The line number in the calling file.
 * \returns         A pointer to the allocated memory.
 */
void * gds_xmalloc(const size_t size,
                   const char * filename, const int linenum);

/*!
 * \brief           Wraps calloc() and aborts on failure.
 * \details         This is designed to be called from the corresponding
 * macro.
 * \param nmemb     The number of members to allocate.
 * \param size      The size in bytes of each member.
 * \param filename  The name of the calling file.
 * \param linenum   The line number in the calling file.
 * \returns         A pointer to the allocated memory.
 */
void * gds_xcalloc(const size_t nmemb, const size_t size,
                   const char * filename, const int linenum);

/*!
 * \brief           Wraps realloc() and aborts on failure.
 * \details         This is designed to be called from the corresponding
 * macro.
 * \param ptr       A pointer to the memory to reallocate.
 * \param size      The number of bytes for the new allocation.
 * \param filename  The name of the calling file.
 * \param linenum   The line number in the calling file.
 * \returns         A pointer to the reallocated memory.
 */
void * gds_xrealloc(void * ptr, const size_t size,
                    const char * filename, const int linenum);

/*!
 * \brief           Wraps strdup() and aborts on failure.
 * \details         This is designed to be called from the corresponding
 * macro.
 * \param str       The string to duplicate.
 * \param filename  The name of the calling file.
 * \param linenum   The line number in the calling file.
 * \returns         A pointer to the allocated memory.
 */
char * gds_xstrdup(const char * str,
                   const char * filename, const int linenum);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GDS_UTIL_STD_WRAPPERS_H  */
