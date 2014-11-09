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

/*!
 * \brief           Opaque stack type definition
 * \ingroup         stack
 */
typedef struct stack * Stack;

/*!
 * \brief           Creates a new stack.
 * \ingroup         stack
 * \param capacity  The initial capacity of the stack.
 * \param type      The datatype for the stack.
 * \param opts      The following options can be OR'd together:
 * `GDS_RESIZABLE` to dynamically resize the stack on-demand;
 * `GDS_FREE_ON_DESTROY` to automatically `free()` pointer members
 * when they are deleted or when the stack is destroyed;
 * `GDS_EXIT_ON_ERROR` to print a message to the standard error stream
 * and `exit()`, rather than returning a failure status.
 * \retval NULL     Stack creation failed.
 * \retval non-NULL A pointer to the new stack.
 */
Stack stack_create(const size_t capacity,
                   const enum gds_datatype type,
                   const int opts);

/*!
 * \brief           Destroys a stack.
 * \details         If the `GDS_FREE_ON_DESTROY` option was specified
 * when creating the stack, any pointer values still in the stack will
 * be `free()`d prior to destruction.
 * \ingroup         stack
 * \param stack     A pointer to the stack.
 */
void stack_destroy(Stack stack);

/*!
 * \brief           Pushes a value onto the stack.
 * \ingroup         stack
 * \param stack     A pointer to the stack.
 * \param ...       The value to push onto the stack. This should
 * be of a type appropriate to the type set when creating the stack.
 * \retval true     Success
 * \retval false    Failure, either because the stack is full or,
 * if the `GDS_RESIZABLE` option was specified when creating the
 * stack, because dynamic memory reallocation failed.
 */
bool stack_push(Stack stack, ...);

/*!
 * \brief           Pops a value from the stack.
 * \ingroup         stack
 * \param stack     A pointer to the stack.
 * \param p         A pointer to an object of a type appropriate to the
 * type set when creating the stack. The object at this address will be
 * modified to contain the value popped from the stack.
 * \retval true     Success
 * \retval false    Failure, stack is empty.
 */
bool stack_pop(Stack stack, void * p);

/*!
 * \brief           Peeks at the top value of the stack.
 * \details         This function retrieves the value which would be
 * popped from the stack, without actually popping it. 
 * \ingroup         stack
 * \param stack     A pointer to the stack.
 * \param p         A pointer to an object of a type appropriate to the
 * type set when creating the stack. The object at this address will be
 * modified to contain the value at the top of the stack.
 * \retval true     Success
 * \retval false    Failure, stack is empty.
 */
bool stack_peek(Stack stack, void * p);

/*!
 * \brief           Checks whether a stack is full.
 * \ingroup         stack
 * \param stack     A pointer to the stack.
 * \retval true     Stack is full
 * \retval false    Stack is not full
 */
bool stack_is_full(Stack stack);

/*!
 * \brief           Checks whether a stack is empty.
 * \ingroup         stack
 * \param stack     A pointer to the stack.
 * \retval true     Stack is empty
 * \retval false    Stack is not empty
 */
bool stack_is_empty(Stack stack);

/*!
 * \brief           Retrieves the current capacity of a stack.
 * \details         This value can change dynamically if the `GDS_RESIZABLE`
 * option was specified when creating the stack.
 * \ingroup         stack
 * \param stack     A pointer to the stack.
 * \returns         The capacity of the stack.
 */
size_t stack_capacity(Stack stack);

/*!
 * \brief           Retrieves the current size of a stack.
 * \details         The size of the stack is equivalent to the number of
 * values currently in it.
 * \ingroup         stack
 * \param stack     A pointer to the stack.
 * \returns         The size of the stack.
 */
size_t stack_size(Stack stack);

/*!
 * \brief           Retrieves the free space on a stack.
 * \details         The free space on a stack is equivalent to the capacity
 * of the stack less the size of the stack. 
 * \ingroup         stack
 * \param stack     A pointer to the stack.
 * \returns         The free space on the stack.
 */
size_t stack_free_space(Stack stack);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GENERIC_STACK_H  */
