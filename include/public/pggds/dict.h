/*!
 * \file            dict.h
 * \brief           Interface to generic dictionary data structure.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GENERIC_DICTIONARY_H
#define PG_GENERIC_DATA_STRUCTURES_GENERIC_DICTIONARY_H

#include <stdbool.h>
#include <stddef.h>

#include "gds_public_types.h"

/*!
 * \brief           Opaque dictionary type definition
 * \ingroup         dict
 */
typedef struct dict * Dict;

/*!
 * \brief           Creates a new dictionary.
 * \ingroup         dict
 * \param type      The datatype for the dictionary.
 * \param opts      The following options can be OR'd together:
 * `GDS_FREE_ON_DESTROY` to automatically `free()` pointer members
 * when they are deleted or when the dictionary is destroyed;
 * `GDS_EXIT_ON_ERROR` to print a message to the standard error stream
 * and `exit()`, rather than returning a failure status.
 * \retval NULL     Dictionart creation failed.
 * \retval non-NULL A pointer to the new dictionary.
 */
Dict dict_create(const enum gds_datatype type,
                 const int opts);

/*!
 * \brief           Destroys a dictionary.
 * \details         If the `GDS_FREE_ON_DESTROY` option was specified
 * when creating the dictionary, any pointer values still in the dictionary
 * will be `free()`d prior to destruction.
 * \ingroup         dict
 * \param dict      A pointer to the dictionary.
 */
void dict_destroy(Dict dict);

/*!
 * \brief           Inserts a key-value into a dictionary.
 * \details         If the key already exists in the dictionary, the
 * existing value will be overwritten. If `GDS_FREE_ON_DESTROY` was
 * specified during dictionary creation, the existing element will be
 * `free()`d prior to overwriting it.
 * \ingroup         dict
 * \param dict      A pointer to the dictionary.
 * \param key       The key.
 * \param ...       The value corresponding to the key. This should
 * be of a type appropriate to the type set when creating the dictionary.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed
 */
bool dict_insert(Dict dict, const char * key, ...);

/*!
 * \brief           Checks whether a key exists in a dictionary.
 * \ingroup         dict
 * \param dict      A pointer to the dictionary.
 * \param key       The key for which to search.
 * \retval true     The key exists in the dictionary
 * \retval false    The key does not exist in the dictionary
 */
bool dict_has_key(Dict dict, const char * key);

/*!
 * \brief           Retrieves the value for a key in the dictionary.
 * \ingroup         dict
 * \param dict      A pointer to the dictionary.
 * \param key       The key for which to retrieve the value.
 * \param p         A pointer to an object of a type appropriate to the
 * type set when creating the dictionary. The object at this address will
 * be modified to contain the value for the specified key.
 * \retval true     Success
 * \retval false    Failure, key was not found
 */
bool dict_value_for_key(Dict dict, const char * key, void * p);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GENERIC_DICTIONARY_H  */
