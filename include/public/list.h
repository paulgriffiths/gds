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

/*!
 * \brief           Opaque list type definition
 * \ingroup         list
 */
typedef struct list * List;

/*!
 * \brief           Opaque list iterator type definition
 * \ingroup         list
 */
typedef struct list_node * ListItr;

/*!
 * \brief           Creates a new list.
 * \ingroup         list
 * \param type      The datatype for the list.
 * \param opts      The following options can be OR'd together:
 * `GDS_FREE_ON_DESTROY` to automatically `free()` pointer members
 * when they are deleted or when the list is destroyed;
 * `GDS_EXIT_ON_ERROR` to print a message to the standard error stream
 * and `exit()`, rather than returning a failure status.
 * \param ...       If `type` is `DATATYPE_POINTER`, this argument should
 * be a pointer to a comparison function. In all other cases, this argument
 * is not required, and will be ignored if it is provided.
 * \retval NULL     List creation failed.
 * \retval non-NULL A pointer to the new list.
 */
List list_create(const enum gds_datatype type, const int opts, ...);

/*!
 * \brief           Destroys a list.
 * \details         If the `GDS_FREE_ON_DESTROY` option was specified
 * when creating the list, any pointer values still in the list will
 * be `free()`d prior to destruction.
 * \ingroup         list
 * \param list      A pointer to the list.
 */
void list_destroy(List list);

/*!
 * \brief           Appends a value to the back of a list.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \param ...       The value to append to the end of the list. This should
 * be of a type appropriate to the type set when creating the list.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed.
 */
bool list_append(List list, ...);

/*!
 * \brief           Prepends a value to the front of a list.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \param ...       The value to prepend to the start of the list. This should
 * be of a type appropriate to the type set when creating the list.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed.
 */
bool list_prepend(List list, ...);

/*!
 * \brief           Inserts a value into a list.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \param index     The index at which to insert the value.
 * \param ...       The value to insert into the list. This should
 * be of a type appropriate to the type set when creating the list.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed or index
 * was out of range.
 */
bool list_insert(List list, const size_t index, ...);

/*!
 * \brief           Deletes the value at the front of the list.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed.
 */
bool list_delete_front(List list);

/*!
 * \brief           Deletes the value at the back of the list.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed.
 */
bool list_delete_back(List list);

/*!
 * \brief           Deletes the value at the specified index of the list.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \param index     The index of the value to delete.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed or index
 * was out of range.
 */
bool list_delete_index(List list, const size_t index);

/*!
 * \brief           Gets the value at the specified index of the list.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \param index     The index of the value to get.
 * \param p         A pointer to an object of a type appropriate to the
 * type set when creating the list. The object at this address will be
 * modified to contain the value at the specified index.
 * \retval true     Success
 * \retval false    Failure, index was out of range.
 */
bool list_element_at_index(List list, const size_t index, void * p);

/*!
 * \brief           Sets the value at the specified index of the list.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \param index     The index of the value to set.
 * \param ...       The value to which to set the specified index of the
 * list. This should be of a type appropriate to the type set when
 * creating the list.
 * \retval true     Success
 * \retval false    Failure, index was out of range.
 */
bool list_set_element_at_index(List list, const size_t index, ...);

/*!
 * \brief           Tests if a value is contained in a list.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \param index     A pointer to a `size_t` object which, if the value
 * is contained within the list, will be modified to contain the index
 * of the first occurrence of that value in the list.
 * \param ...       The value for which to search. This should be of a
 * type appropriate to the type set when creating the list.
 * \retval true     The value was found in the list
 * \retval false    The value was not found in the list
 */
bool list_find(List list, size_t * index, ...);

/*!
 * \brief           Tests if a value is contained in a list.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \param ...       The value for which to search. This should be of a
 * type appropriate to the type set when creating the list.
 * \retval NULL     The value was not found in the list
 * \retval non-NULL A list iterator pointing to the first occurrence of
 * the vaue in the list.
 */
ListItr list_find_itr(List list, ...);

/*!
 * \brief           Sorts a list in-place, in ascending order.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed.
 */
bool list_sort(List list);

/*!
 * \brief           Sorts a list in-place, in descending order.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed.
 */
bool list_reverse_sort(List list);

/*!
 * \brief           Returns an iterator to the first element of the list.
 * \ingroup         list
 * \param list      A pointer to the list
 * \retval NULL     Failure, list is empty
 * \retval non-NULL An iterator to the first element of the list
 */
ListItr list_itr_first(List list);

/*!
 * \brief           Returns an iterator to the last element of the list.
 * \ingroup         list
 * \param list      A pointer to the list
 * \retval NULL     Failure, list is empty
 * \retval non-NULL An iterator to the last element of the list
 */
ListItr list_itr_last(List list);

/*!
 * \brief           Increments a list iterator.
 * \ingroup         list
 * \param itr       A pointer to the iterator.
 * \retval NULL     End of list, no next iterator
 * \retval non-NULL An iterator to the next element of the list
 */
ListItr list_itr_next(ListItr itr);

/*!
 * \brief           Decrements a list iterator.
 * \ingroup         list
 * \param itr       A pointer to the iterator.
 * \retval NULL     Start of list, no previous iterator
 * \retval non-NULL An iterator to the previous element of the list
 */
ListItr list_itr_previous(ListItr itr);

/*!
 * \brief           Retrieves a value from an iterator.
 * \ingroup         list
 * \param itr       A pointer to the iterator.
 * \param p         A pointer to an object of a type appropriate to the
 * type set when creating the list. The object at this address will be
 * modified to contain the value at the given iterator.
 */
void list_value_at_itr(ListItr itr, void * p);

/*!
 * \brief           Tests if a list is empty.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \retval true     The list is empty
 * \retval false    The list is not empty
 */
bool list_is_empty(List list);

/*!
 * \brief           Returns the length of a list.
 * \details         The length of the list is equivalent to the number of
 * values it contains.
 * \ingroup         list
 * \param list      A pointer to the list.
 * \returns         The length of the list.
 */
size_t list_length(List list);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GENERIC_LIST_H  */
