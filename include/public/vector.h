/*!
 * \file            vector.h
 * \brief           Interface to generic vector data structure.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GENERIC_VECTOR_H
#define PG_GENERIC_DATA_STRUCTURES_GENERIC_VECTOR_H

#include <stdbool.h>
#include <stddef.h>

#include "gds_public_types.h"

/*!
 * \brief           Opaque vector type definition
 * \ingroup         vector
 */
typedef struct vector * Vector;

/*!
 * \brief           Creates a new vector.
 * \ingroup         vector
 * \param capacity  The initial capacity for the vector.
 * \param type      The datatype for the vector.
 * \param opts      The following options can be OR'd together:
 * `GDS_FREE_ON_DESTROY` to automatically `free()` pointer members
 * when they are deleted or when the vector is destroyed;
 * `GDS_EXIT_ON_ERROR` to print a message to the standard error stream
 * and `exit()`, rather than returning a failure status.
 * \param ...       If `type` is `DATATYPE_POINTER`, this argument should
 * be a pointer to a comparison function. In all other cases, this argument
 * is not required, and will be ignored if it is provided.
 * \retval NULL     Vector creation failed.
 * \retval non-NULL A pointer to the new vector.
 */
Vector vector_create(const size_t capacity,
                     const enum gds_datatype type,
                     const int opts, ...);

/*!
 * \brief           Destroys a vector.
 * \details         If the `GDS_FREE_ON_DESTROY` option was specified
 * when creating the vector, any pointer values still in the vector will
 * be `free()`d prior to destruction.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 */
void vector_destroy(Vector vector);

/*!
 * \brief           Appends a value to the back of a vector.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 * \param ...       The value to append to the end of the vector. This should
 * be of a type appropriate to the type set when creating the vector.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed.
 */
bool vector_append(Vector vector, ...);

/*!
 * \brief           Prepends a value to the front of a vector.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 * \param ...       The value to prepend to the start of the vector. This
 * should be of a type appropriate to the type set when creating the vector.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed.
 */
bool vector_prepend(Vector vector, ...);

/*!
 * \brief           Inserts a value into a vector.
 * \ingroup         vector
 * \param vector    A pointer to the list.
 * \param index     The index at which to insert the value.
 * \param ...       The value to insert into the vector. This should
 * be of a type appropriate to the type set when creating the vector.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed or index
 * was out of range.
 */
bool vector_insert(Vector vector, const size_t index, ...);

/*!
 * \brief           Deletes the value at the front of the vector.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed.
 */
bool vector_delete_front(Vector vector);

/*!
 * \brief           Deletes the value at the back of the vector.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed.
 */
bool vector_delete_back(Vector vector);

/*!
 * \brief           Deletes the value at the specified index of the vector.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 * \param index     The index of the value to delete.
 * \retval true     Success
 * \retval false    Failure, dynamic memory allocation failed or index
 * was out of range.
 */
bool vector_delete_index(Vector vector, const size_t index);

/*!
 * \brief           Gets the value at the specified index of the vector.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 * \param index     The index of the value to get.
 * \param p         A pointer to an object of a type appropriate to the
 * type set when creating the vector. The object at this address will be
 * modified to contain the value at the specified index.
 * \retval true     Success
 * \retval false    Failure, index was out of range.
 */
bool vector_element_at_index(Vector vector, const size_t index, void * p);

/*!
 * \brief           Sets the value at the specified index of the vector.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 * \param index     The index of the value to set.
 * \param ...       The value to which to set the specified index of the
 * vector. This should be of a type appropriate to the type set when
 * creating the vector.
 * \retval true     Success
 * \retval false    Failure, index was out of range.
 */
bool vector_set_element_at_index(Vector vector, const size_t index, ...);

/*!
 * \brief           Tests if a value is contained in a vector.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 * \param index     A pointer to a `size_t` object which, if the value
 * is contained within the vector, will be modified to contain the index
 * of the first occurrence of that value in the vector.
 * \param ...       The value for which to search. This should be of a
 * type appropriate to the type set when creating the vector.
 * \retval true     The value was found in the vector
 * \retval false    The value was not found in the vector
 */
bool vector_find(Vector vector, size_t * index, ...);

/*!
 * \brief           Sorts a vector in-place, in ascending order.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 */
void vector_sort(Vector vector);

/*!
 * \brief           Sorts a vector in-place, in descending order.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 */
void vector_reverse_sort(Vector vector);

/*!
 * \brief           Tests if a vector is empty.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 * \retval true     The vector is empty
 * \retval false    The vector is not empty
 */
bool vector_is_empty(Vector vector);

/*!
 * \brief           Returns the length of a vector.
 * \details         The length of the vector is equivalent to the number of
 * values it contains. This can be less than the initial capacity, and as
 * low as zero, if elements have been deleted from the vector.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 * \returns         The length of the vector.
 */
size_t vector_length(Vector vector);

/*!
 * \brief           Returns the capacity of a vector.
 * \details         The capacity of the vector is equivalent to the number of
 * values it is capable of holding. This value can dynamically change if a
 * vector resizes to append an element at the back of the vector. The capacity
 * does not change when elements are deleted from a vector.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 * \returns         The capacity of the vector.
 */
size_t vector_capacity(Vector vector);

/*!
 * \brief           Returns the free space in a vector.
 * \details         The free space in a vector is equivalent to its
 * capacity less its length. The free space can change if a vector
 * dynamically resizes to append an element at the back of the vector,
 * or if elements are deleted from the vector.
 * \ingroup         vector
 * \param vector    A pointer to the vector.
 * \returns         The free space in the vector.
 */
size_t vector_free_space(Vector vector);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GENERIC_VECTOR_H  */
