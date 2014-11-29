/*!
 * \file            queue.h
 * \brief           Interface to generic queue data structure.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GENERIC_QUEUE_H
#define PG_GENERIC_DATA_STRUCTURES_GENERIC_QUEUE_H

#include <stdbool.h>
#include <stddef.h>

#include "gds_public_types.h"

/*!
 * \brief           Opaque queue type definition
 * \ingroup         queue
 */
typedef struct queue * Queue;

/*!
 * \brief           Creates a new queue.
 * \ingroup         queue
 * \param capacity  The initial capacity of the queue.
 * \param type      The datatype for the queue.
 * \param opts      The following options can be OR'd together:
 * `GDS_RESIZABLE` to dynamically resize the queue on-demand;
 * `GDS_FREE_ON_DESTROY` to automatically `free()` pointer members
 * when they are deleted or when the queue is destroyed;
 * `GDS_EXIT_ON_ERROR` to print a message to the standard error stream
 * and `exit()`, rather than returning a failure status.
 * \retval NULL     Queue creation failed.
 * \retval non-NULL A pointer to the new queue.
 */
Queue queue_create(const size_t capacity,
                   const enum gds_datatype type,
                   const int opts);

/*!
 * \brief           Destroys a queue.
 * \details         If the `GDS_FREE_ON_DESTROY` option was specified
 * when creating the queue, any pointer values still in the queue will
 * be `free()`d prior to destruction.
 * \ingroup         queue
 * \param queue     A pointer to the queue.
 */
void queue_destroy(Queue queue);

/*!
 * \brief           Pushes a value onto the queue.
 * \ingroup         queue
 * \param queue     A pointer to the queue.
 * \param ...       The value to push onto the queue. This should
 * be of a type appropriate to the type set when creating the queue.
 * \retval true     Success
 * \retval false    Failure, either because the queue is full or,
 * if the `GDS_RESIZABLE` option was specified when creating the
 * queue, because dynamic memory reallocation failed.
 */
bool queue_push(Queue queue, ...);

/*!
 * \brief           Pops a value from the queue.
 * \ingroup         queue
 * \param queue     A pointer to the queue.
 * \param p         A pointer to an object of a type appropriate to the
 * type set when creating the queue. The object at this address will be
 * modified to contain the value popped from the queue.
 * \retval true     Success
 * \retval false    Failure, queue is empty.
 */
bool queue_pop(Queue queue, void * p);

/*!
 * \brief           Peeks at the top value of the queue.
 * \details         This function retrieves the value which would be
 * popped from the queue, without actually popping it. 
 * \ingroup         queue
 * \param queue     A pointer to the queue.
 * \param p         A pointer to an object of a type appropriate to the
 * type set when creating the queue. The object at this address will be
 * modified to contain the value at the top of the queue.
 * \retval true     Success
 * \retval false    Failure, queue is empty.
 */
bool queue_peek(Queue queue, void * p);

/*!
 * \brief           Checks whether a queue is full.
 * \ingroup         queue
 * \param queue     A pointer to the queue.
 * \retval true     Queue is full
 * \retval false    Queue is not full
 */
bool queue_is_full(Queue queue);

/*!
 * \brief           Checks whether a queue is empty.
 * \ingroup         queue
 * \param queue     A pointer to the queue.
 * \retval true     Queue is empty
 * \retval false    Queue is not empty
 */
bool queue_is_empty(Queue queue);

/*!
 * \brief           Retrieves the current capacity of a queue.
 * \details         This value can change dynamically if the `GDS_RESIZABLE`
 * option was specified when creating the queue.
 * \ingroup         queue
 * \param queue     A pointer to the queue.
 * \returns         The capacity of the queue.
 */
size_t queue_capacity(Queue queue);

/*!
 * \brief           Retrieves the current size of a queue.
 * \details         The size of the queue is equivalent to the number of
 * values currently in it.
 * \ingroup         queue
 * \param queue     A pointer to the queue.
 * \returns         The size of the queue.
 */
size_t queue_size(Queue queue);

/*!
 * \brief           Retrieves the free space on a queue.
 * \details         The free space on a queue is equivalent to the capacity
 * of the queue less the size of the queue. 
 * \ingroup         queue
 * \param queue     A pointer to the queue.
 * \returns         The free space on the queue.
 */
size_t queue_free_space(Queue queue);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GENERIC_QUEUE_H  */
