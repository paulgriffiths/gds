/*!
 * \file            list.h
 * \brief           Interface to generic list data structure.
 * \details         The list is implemented as a double-ended, double-linked
 * list.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GENERIC_LIST_H
#define PG_GENERIC_DATA_STRUCTURES_GENERIC_LIST_H

#include <stdbool.h>
#include <stddef.h>

#include "gds_public_types.h"

/*!  Opaque list type definition  */
typedef struct list * List;

/*!
 * \brief           Creates a new list.
 * \param type      The datatype for the list.
 * \param opts      The following options can be OR'd together:
 * GDS_FREE_ON_DESTROY to automatically `free()` pointer members
 * when they are deleted or when the list is destroyed;
 * GDS_EXIT_ON_ERROR to print a message to the standard error stream
 * and `exit()`, rather than returning a failure status.
 * \param ...       If `type` is `DATATYPE_POINTER`, this argument should
 * be a pointer to a comparison function. In all other cases, this argument
 * is not required, and will be ignored if it is provided.
 * \retval NULL     List creation failed.
 * \retval non-NULL A pointer to the new list.
 */
List list_create(const enum gds_datatype type, const int opts, ...);

void list_destroy(List list);

bool list_append(List list, ...);
bool list_prepend(List list, ...);
bool list_insert(List list, const size_t index, ...);

bool list_delete_index(List list, const size_t index);
bool list_delete_front(List list);
bool list_delete_back(List list);

bool list_element_at_index(List list, const size_t index, void * p);
bool list_set_element_at_index(List list, const size_t index, ...);

bool list_find(List list, size_t * index, ...);

bool list_is_empty(List list);
size_t list_length(List list);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GENERIC_LIST_H  */
