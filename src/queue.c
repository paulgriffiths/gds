#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "gds_common.h"
#include "queue.h"

static const size_t GROWTH = 2;

struct queue {
    size_t front;
    size_t back;
    size_t capacity;
    size_t size;

    enum gds_datatype type;
    struct gdt_generic_datatype * elements;

    bool resizable;
    bool free_on_destroy;
    bool exit_on_error;
};

/*  Creates and returns a new queue of specified type and capacity  */

struct queue * queue_create(const size_t capacity,
                            const enum gds_datatype type,
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

void queue_destroy(struct queue * queue)
{
    if ( queue->free_on_destroy ) {
        while ( queue->size ) {
            gdt_free(&queue->elements[queue->front++], queue->type);
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

bool queue_push(struct queue * queue, ...)
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

/*  Pops an element from the queue  */

bool queue_pop(struct queue * queue, void * p)
{
    if ( queue_is_empty(queue) ) {
        if ( queue->exit_on_error ) {
            gds_error_quit("queue empty (%s, line %d)", __FILE__, __LINE__);
        }
        else {
            return false;
        }
    }

    gdt_get_value(&queue->elements[queue->front++], queue->type, p);

    if ( queue->front == queue->capacity ) {
        queue->front = 0;
    }

    queue->size -= 1;

    return true;
}

/*  Peeks at the front element of the queue without popping it  */

bool queue_peek(struct queue * queue, void * p)
{
    if ( queue_is_empty(queue) ) {
        if ( queue->exit_on_error ) {
            gds_error_quit("queue empty (%s, line %d)", __FILE__, __LINE__);
        }
        else {
            return false;
        }
    }

    gdt_get_value(&queue->elements[queue->front], queue->type, p);

    return true;
}

/*  Returns true if the queue is full  */

bool queue_is_full(struct queue * queue)
{
    return queue->size == queue->capacity;
}

/*  Returns true if the queue is empty  */

bool queue_is_empty(struct queue * queue)
{
    return queue->size == 0;
}

/*  Returns the capacity of a queue  */

size_t queue_capacity(struct queue * queue)
{
    return queue->capacity;
}

/*  Returns the number of free elements on the queue  */

size_t queue_free_space(struct queue * queue)
{
    return queue->capacity - queue->size;
}

/*  Returns the number of elements on the queue  */

size_t queue_size(struct queue * queue)
{
    return queue->size;
}
