#ifndef PG_SAMPLES_AND_DEMOS_GENERIC_QUEUE_H
#define PG_SAMPLES_AND_DEMOS_GENERIC_QUEUE_H

#include <stdbool.h>
#include <stddef.h>

#include "gds_public_types.h"

typedef struct queue * Queue;

Queue queue_create(const size_t capacity,
                   const enum gds_datatype type,
                   const int opts);
void queue_destroy(Queue queue);
bool queue_push(Queue queue, ...);
bool queue_pop(Queue queue, void * p);
bool queue_peek(Queue queue, void * p);
bool queue_is_full(Queue queue);
bool queue_is_empty(Queue queue);
size_t queue_capacity(Queue queue);
size_t queue_free_space(Queue queue);
size_t queue_size(Queue queue);

#endif      /*  PG_SAMPLES_AND_DEMOS_GENERIC_QUEUE_H  */
