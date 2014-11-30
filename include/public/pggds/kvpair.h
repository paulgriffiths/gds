/*!
 * \file            kvpair.h
 * \brief           Interface to generic key-value pair structure.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GENERIC_KVPAIR_H
#define PG_GENERIC_DATA_STRUCTURES_GENERIC_KVPAIR_H

#include <stdbool.h>
#include <pggds/gds_public_types.h>

/*!  Key-Value pair structure  */
typedef struct gds_kvpair {
    char * key;                               /*!<  String key              */
    struct gdt_generic_datatype value;        /*!<  Generic datatype value  */
} * KVPair;

/*!
 * \brief           Creates a new key-value pair.
 * \param key       The key for the new pair.
 * \param type      The datatype for the new pair
 * \param ap        A `va_list` containing the data value for the pair.
 * This should be of a type appropriate to the type set when creating
 * the list.
 * \retval NULL     Failure, dynamic memory allocation failed
 * \retval non-NULL Success
 */
KVPair gds_kvpair_create(const char * key,
                         const enum gds_datatype type,
                         va_list ap);

/*!
 * \brief               Destroys a key-value pair.
 * \param pair          A pointer to the pair to destroy.
 * \param free_value    If true, the data will be passed to gdt_free()
 */ 
void gds_kvpair_destroy(KVPair pair, const bool free_value);

/*!
 * \brief               Compares two key-value pairs by key.
 * \details             This function is suitable for passing to qsort().
 * \param p1            A pointer to the first pair.
 * \param p2            A pointer to the second pair.
 * \retval 0            The keys of the two pairs are equal
 * \retval -1           The key of the first pair is less than the key
 * of the second pair
 * \retval 1            The key of the first pair is greater than the key
 * of the second pair
 */
int gds_kvpair_compare(const void * p1, const void * p2);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GENERIC_KVPAIR_H  */
