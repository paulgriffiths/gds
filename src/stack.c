/*!
 * \file            stack.c
 * \brief           Implementation of generic stack data structure.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pggds_internal/gds_common.h>
#include <pggds/stack.h>

/*!  Growth factor for dynamic memory allocation  */
static const size_t GROWTH = 2;

/*!  Stack structure  */
struct stack {
    size_t top;                                 /*!<  Top of stack          */
    size_t capacity;                            /*!<  Stack capacity        */
    enum gds_datatype type;                     /*!<  Stack datatype        */
    struct gdt_generic_datatype * elements;     /*!<  Pointer to elements   */
    bool resizable;         /*!<  Dynamically resizabe if true              */
    bool free_on_destroy;   /*!<  Free pointer elements on destroy if true  */
    bool exit_on_error;     /*!<  Exit on error if true                     */
};

Stack stack_create(const size_t capacity, const enum gds_datatype type,
                   const int opts)
{
    struct stack * new_stack = malloc(sizeof *new_stack);
    if ( !new_stack ) {
        if ( opts & GDS_EXIT_ON_ERROR ) {
            quit_strerror("gds library", "memory allocation failed");
        }
        else {
            return NULL;
        }
    }

    new_stack->capacity = capacity;
    new_stack->top = 0;
    new_stack->type = type;
    new_stack->resizable = (opts & GDS_RESIZABLE) ? true : false;
    new_stack->free_on_destroy = (opts & GDS_FREE_ON_DESTROY) ? true : false;
    new_stack->exit_on_error = (opts & GDS_EXIT_ON_ERROR) ? true : false;

    new_stack->elements = malloc(sizeof *new_stack->elements * capacity);
    if ( !new_stack->elements ) {
        if ( new_stack->exit_on_error ) {
            quit_strerror("gds library", "memory allocation failed");
        }
        else {
            free(new_stack);
            return NULL;
        }
    }

    return new_stack;
}

void stack_destroy(Stack stack)
{
    if ( stack->free_on_destroy ) {

        /*  We can't use stack_pop() to get each element in
         *  turn, here, since that would require knowing
         *  their type and break encapsulation, so we have
         *  to do it manually and tolerate a little code
         *  duplication for the greater good.                */

        while ( stack->top ) {
            gdt_free(&stack->elements[--stack->top]);
        }
    }

    free(stack->elements);
    free(stack);
}

bool stack_push(Stack stack, ...)
{
    if ( stack_is_full(stack) ) {
        if ( stack->resizable ) {
            struct gdt_generic_datatype * new_elements;
            const size_t new_capacity = stack->capacity * GROWTH;

            new_elements = realloc(stack->elements,
                                   sizeof *stack->elements * new_capacity);
            if ( !new_elements ) {
                if ( stack->exit_on_error ) {
                    quit_strerror("gds library", "memory allocation failed");
                }
                else {
                    return false;
                }
            }

            stack->elements = new_elements;
            stack->capacity = new_capacity;
        }
        else if ( stack->exit_on_error ) {
            quit_error("gds library", "stack full");
        }
        else {
            return false;
        }
    }

    va_list ap;
    va_start(ap, stack);
    gdt_set_value(&stack->elements[stack->top++], stack->type, NULL, ap);
    va_end(ap);

    return true;
}

bool stack_pop(Stack stack, void * p)
{
    if ( stack_is_empty(stack) ) {
        if ( stack->exit_on_error ) {
            quit_error("gds library", "stack empty");
        }
        else {
            return false;
        }
    }

    gdt_get_value(&stack->elements[--stack->top], p);

    return true;
}

bool stack_peek(Stack stack, void * p)
{
    if ( stack_is_empty(stack) ) {
        if ( stack->exit_on_error ) {
            quit_error("gds library", "stack empty");
        }
        else {
            return false;
        }
    }

    gdt_get_value(&stack->elements[stack->top - 1], p);

    return true;
}

bool stack_is_full(Stack stack)
{
    return stack->top == stack->capacity;
}

bool stack_is_empty(Stack stack)
{
    return stack->top == 0;
}

size_t stack_capacity(Stack stack)
{
    return stack->capacity;
}

size_t stack_free_space(Stack stack)
{
    return stack->capacity - stack->top;
}

size_t stack_size(Stack stack)
{
    return stack->top;
}
