#ifndef PG_SAMPLES_AND_DEMOS_GENERIC_LIST_H
#define PG_SAMPLES_AND_DEMOS_GENERIC_LIST_H

#include <stdbool.h>
#include <stddef.h>

#include "gds_public_types.h"

typedef struct list * List;

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

#endif      /*  PG_SAMPLES_AND_DEMOS_GENERIC_LIST_H  */
