/*!
 * \file            stack.h
 * \brief           Interface to generic stack data structure.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GENERIC_STACK_H
#define PG_GENERIC_DATA_STRUCTURES_GENERIC_STACK_H

#include <stdbool.h>
#include <stddef.h>

#include "gds_public_types.h"

typedef struct stack * Stack;

Stack stack_create(const size_t capacity,
                   const enum gds_datatype type,
                   const int opts);
void stack_destroy(Stack stack);
bool stack_push(Stack stack, ...);
bool stack_pop(Stack stack, void * p);
bool stack_peek(Stack stack, void * p);
bool stack_is_full(Stack stack);
bool stack_is_empty(Stack stack);
size_t stack_capacity(Stack stack);
size_t stack_free_space(Stack stack);
size_t stack_size(Stack stack);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GENERIC_STACK_H  */
