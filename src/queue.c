/*!
 * \file            queue.c
 * \brief           Implementation of generic queue data structure.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "gds_common.h"
#include "queue.h"

/*!
 * \brief           Growth factor for dynamic memory allocation
 * \attention       queue_push() relies on this being at least 2.
 */
static const size_t GROWTH = 2;

/*!  Queue structure  */
struct queue {
    size_t front;                               /*!<  Front of queue        */
    size_t back;                                /*!<  Back of queue         */
    size_t capacity;                            /*!<  Capacity of queue     */
    size_t size;                                /*!<  Size of queue         */

    enum gds_datatype type;                     /*!<  Queue datatype        */
    struct gdt_generic_datatype * elements;     /*!<  Pointer to elements   */

    bool resizable;         /*!<  Dynamically resizable if true             */
    bool free_on_destroy;   /*!<  Free pointer elements on destroy if true  */
    bool exit_on_error;     /*!<  Exit on error if true                     */
};

Queue queue_create(const size_t capacity, const enum gds_datatype type,
                   const int opts)
{
    struct queue * new_queue = malloc(sizeof *new_queue);
    if ( !new_queue ) {
        if ( opts & GDS_EXIT_ON_ERROR ) {
            gds_strerror_quit("memory allocation failed "
                              "(%s, line %d)", __FILE__, __LINE__);
        }
        else {
            return NULL;
        }
    }

    new_queue->front = 0;
    new_queue->back = 0;
    new_queue->capacity = capacity;
    new_queue->size = 0;
    new_queue->type = type;

    new_queue->resizable = (opts & GDS_RESIZABLE) ? true : false;
    new_queue->free_on_destroy = (opts & GDS_FREE_ON_DESTROY) ? true : false;
    new_queue->exit_on_error = (opts & GDS_EXIT_ON_ERROR) ? true : false;

    new_queue->elements = malloc(sizeof *new_queue->elements * capacity);
    if ( !new_queue->elements ) {
        if ( new_queue->exit_on_error ) {
            gds_strerror_quit("memory allocation failed "
                              "(%s, line %d)", __FILE__, __LINE__);
        }
        else {
            free(new_queue);
            return NULL;
        }
    }

    return new_queue;
}

/*  Destroys a previously created queue  */

void queue_destroy(Queue queue)
{
    if ( queue->free_on_destroy ) {

        /*  We can't use queue_pop() to get each element in
         *  turn, here, since that would require knowing
         *  their type and break encapsulation, so we have
         *  to do it manually and tolerate a little code
         *  duplication for the greater good.                */

        while ( queue->size ) {
            gdt_free(&queue->elements[queue->front++]);
            if ( queue->front == queue->capacity ) {
                queue->front = 0;
            }
            queue->size -= 1;
        }
    }

    free(queue->elements);
    free(queue);
}

/*  Pushes an element onto the queue  */

bool queue_push(Queue queue, ...)
{
    if ( queue_is_full(queue) ) {
        if ( queue->resizable ) {
            struct gdt_generic_datatype * new_elements;
            const size_t new_capacity = queue->capacity * GROWTH;

            new_elements = realloc(queue->elements,
                                   sizeof *queue->elements * new_capacity);
            if ( !new_elements ) {
                if ( queue->exit_on_error ) {
                    gds_strerror_quit("memory reallocation failed "
                                      "(%s, line %d)", __FILE__, __LINE__);
                }
                else {
                    return false;
                }
            }

            queue->elements = new_elements;

            if ( queue->back < queue->front ||
                 queue->size == queue->capacity ) {

                /*  If we get here, then the back of the queue
                 *  is at a lower index than the front of it
                 *  (or the queue is full and both the back and
                 *  front are zero). Conceptually the queue is
                 *  wrapping around the back of the array, and if
                 *  we resize it, there'll be a gap unless we move
                 *  those wrapped elements back into the new space.
                 *  Note that because we always grow by a factor of
                 *  at least two, there'll always be enough space to
                 *  move all the wrapped elements. In fact, we here
                 *  move the entire array from the start through to
                 *  the front element, including any "empty" ones,
                 *  which is not really necessary.                    */

                /**  \todo Rewrite to move only the required elements  */

                const size_t excess = new_capacity - queue->capacity;
                const size_t nfelem = queue->capacity - queue->front;
                struct gdt_generic_datatype * old_front, * new_front;

                old_front = queue->elements + queue->front;
                new_front = old_front + excess;
                memmove(new_front, old_front, nfelem * sizeof *old_front);
                queue->front += excess;
            }

            queue->capacity = new_capacity;
        }
        else if ( queue->exit_on_error ) {
            gds_error_quit("queue full (%s, line %d)", __FILE__, __LINE__);
        }
        else {
            return false;
        }
    }

    va_list ap;
    va_start(ap, queue);
    gdt_set_value(&queue->elements[queue->back++], queue->type, NULL, ap);
    va_end(ap);

    if ( queue->back == queue->capacity ) {
        queue->back = 0;
    }

    queue->size += 1;

    return true;
}

bool queue_pop(Queue queue, void * p)
{
    if ( queue_is_empty(queue) ) {
        if ( queue->exit_on_error ) {
            gds_error_quit("queue empty (%s, line %d)", __FILE__, __LINE__);
        }
        else {
            return false;
        }
    }

    gdt_get_value(&queue->elements[queue->front++], p);

    if ( queue->front == queue->capacity ) {
        queue->front = 0;
    }

    queue->size -= 1;

    return true;
}

bool queue_peek(Queue queue, void * p)
{
    if ( queue_is_empty(queue) ) {
        if ( queue->exit_on_error ) {
            gds_error_quit("queue empty (%s, line %d)", __FILE__, __LINE__);
        }
        else {
            return false;
        }
    }

    gdt_get_value(&queue->elements[queue->front], p);

    return true;
}

bool queue_is_full(Queue queue)
{
    return queue->size == queue->capacity;
}

bool queue_is_empty(Queue queue)
{
    return queue->size == 0;
}

size_t queue_capacity(Queue queue)
{
    return queue->capacity;
}

size_t queue_free_space(Queue queue)
{
    return queue->capacity - queue->size;
}

size_t queue_size(Queue queue)
{
    return queue->size;
}
