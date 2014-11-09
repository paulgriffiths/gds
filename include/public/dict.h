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

bool dict_insert(Dict dict, const char * key, ...);
int dict_has_key(Dict dict, const char * key);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GENERIC_DICTIONARY_H  */
