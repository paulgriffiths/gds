#ifndef PG_SAMPLES_AND_DEMOS_GENERIC_VECTOR_H
#define PG_SAMPLES_AND_DEMOS_GENERIC_VECTOR_H

#include <stdbool.h>
#include <stddef.h>

#include "gds_public_types.h"

typedef struct vector * Vector;

Vector vector_create(const size_t capacity,
                     const enum gds_datatype type,
                     const int opts, ...);
void vector_destroy(Vector vector);

bool vector_append(Vector vector, ...);
bool vector_prepend(Vector vector, ...);
bool vector_insert(Vector vector, const size_t index, ...);

bool vector_delete_index(Vector vector, const size_t index);
bool vector_delete_front(Vector vector);
bool vector_delete_back(Vector vector);

bool vector_element_at_index(Vector vector, const size_t index, void * p);
bool vector_set_element_at_index(Vector vector, const size_t index, ...);

bool vector_find(Vector vector, size_t * index, ...);
void vector_sort(Vector vector);
void vector_reverse_sort(Vector vector);

bool vector_is_empty(Vector vector);
size_t vector_length(Vector vector);
size_t vector_capacity(Vector vector);
size_t vector_free_space(Vector vector);

#endif      /*  PG_SAMPLES_AND_DEMOS_GENERIC_VECTOR_H  */
