#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "gds_common.h"
#include "stack.h"

static const size_t GROWTH = 2;

struct stack {
    size_t top;
    size_t capacity;
    enum gds_datatype type;
    struct gdt_generic_datatype * elements;
    bool resizable;
    bool free_on_destroy;
    bool exit_on_error;
};

/*  Creates and returns a new stack of specified type and capacity  */

struct stack * stack_create(const size_t capacity,
                            const enum gds_datatype type,
                            const int opts)
{
    struct stack * new_stack = malloc(sizeof *new_stack);
    if ( !new_stack ) {
        if ( opts & GDS_EXIT_ON_ERROR ) {
            gds_strerror_quit("memory allocation failed "
                              "(%s, line %d)", __FILE__, __LINE__);
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
            gds_strerror_quit("memory allocation failed "
                              "(%s, line %d)", __FILE__, __LINE__);
        }
        else {
            free(new_stack);
            return NULL;
        }
    }

    return new_stack;
}

/*  Destroys a previously created stack  */

void stack_destroy(struct stack * stack)
{
    if ( stack->free_on_destroy ) {
        while ( stack->top ) {
            gdt_free(&stack->elements[--stack->top], stack->type);
        }
    }

    free(stack->elements);
    free(stack);
}

/*  Pushes an element onto the stack  */

bool stack_push(struct stack * stack, ...)
{
    if ( stack_is_full(stack) ) {
        if ( stack->resizable ) {
            struct gdt_generic_datatype * new_elements;
            const size_t new_capacity = stack->capacity * GROWTH;

            new_elements = realloc(stack->elements,
                                   sizeof *stack->elements * new_capacity);
            if ( !new_elements ) {
                if ( stack->exit_on_error ) {
                    gds_strerror_quit("memory reallocation failed "
                                      "(%s, %d)", __FILE__, __LINE__);
                }
                else {
                    return false;
                }
            }

            stack->elements = new_elements;
            stack->capacity = new_capacity;
        }
        else if ( stack->exit_on_error ) {
            gds_error_quit("stack full (%s, line %d)", __FILE__, __LINE__);
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

/*  Pops an element from the stack  */

bool stack_pop(struct stack * stack, void * p)
{
    if ( stack_is_empty(stack) ) {
        if ( stack->exit_on_error ) {
            gds_error_quit("stack empty (%s, line %d)", __FILE__, __LINE__);
        }
        else {
            return false;
        }
    }

    gdt_get_value(&stack->elements[--stack->top], stack->type, p);

    return true;
}

/*  Peeks at the top element of the stack without popping it  */

bool stack_peek(struct stack * stack, void * p)
{
    if ( stack_is_empty(stack) ) {
        if ( stack->exit_on_error ) {
            gds_error_quit("stack empty (%s, line %d)", __FILE__, __LINE__);
        }
        else {
            return false;
        }
    }

    gdt_get_value(&stack->elements[stack->top - 1], stack->type, p);

    return true;
}

/*  Returns true if the stack is full  */

bool stack_is_full(struct stack * stack)
{
    return stack->top == stack->capacity;
}

/*  Returns true if the stack is empty  */

bool stack_is_empty(struct stack * stack)
{
    return stack->top == 0;
}

/*  Returns the capacity of a stack  */

size_t stack_capacity(struct stack * stack)
{
    return stack->capacity;
}

/*  Returns the number of free elements on the stack  */

size_t stack_free_space(struct stack * stack)
{
    return stack->capacity - stack->top;
}

/*  Returns the number of elements currently on the stack  */

size_t stack_size(struct stack * stack)
{
    return stack->top;
}
