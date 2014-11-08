#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "gds_common.h"
#include "vector.h"

static const size_t GROWTH = 2;

struct vector {
    size_t length;
    size_t capacity;
    enum gds_datatype type;
    struct gdt_generic_datatype * elements;
    int (*compfunc)(const void *, const void *);

    bool free_on_destroy;
    bool exit_on_error;
};

static bool vector_insert_internal(struct vector * vector,
                                   const size_t index, va_list ap);

/*  Creates and returns a new vector of specified type  */

struct vector * vector_create(const size_t capacity,
                              const enum gds_datatype type,
                              const int opts, ...)
{
    struct vector * new_vector = malloc(sizeof *new_vector);
    if ( !new_vector ) {
        if ( opts & GDS_EXIT_ON_ERROR ) {
            gds_strerror_quit("memory allocation failed "
                              "(%s, line %d)", __FILE__, __LINE__);
        }
        else {
            return NULL;
        }
    }

    new_vector->capacity = capacity;
    new_vector->length = capacity;
    new_vector->type = type;
    new_vector->free_on_destroy = (opts & GDS_FREE_ON_DESTROY) ? true : false;
    new_vector->exit_on_error = (opts & GDS_EXIT_ON_ERROR) ? true : false;

    va_list ap;
    va_start(ap, opts);
    if ( type == DATATYPE_POINTER ) {
        new_vector->compfunc = va_arg(ap, int (*)(const void *, const void *));
    }
    else {
        new_vector->compfunc = NULL;
    }
    va_end(ap);

    new_vector->elements = calloc(capacity, sizeof *new_vector->elements);
    if ( !new_vector->elements ) {
        if ( new_vector->exit_on_error ) {
            gds_strerror_quit("memory allocation failed "
                              "(%s, line %d)", __FILE__, __LINE__);
        }
        else {
            free(new_vector);
            return NULL;
        }
    }

    return new_vector;
}

/*  Destroys a previously created vector  */

void vector_destroy(struct vector * vector)
{
    if ( vector->free_on_destroy ) {
        for ( size_t i = 0; i < vector->length; ++i ) {
            gdt_free(&vector->elements[i], vector->type);
        }
    }

    free(vector->elements);
    free(vector);
}

/*  Appends a value to the end of a vector  */

bool vector_append(struct vector * vector, ...)
{
    va_list ap;
    va_start(ap, vector);
    bool status = vector_insert_internal(vector, vector->length, ap);
    va_end(ap);

    return status;
}

/*  Prepends a value to the front of a vector  */

bool vector_prepend(struct vector * vector, ...)
{
    va_list ap;
    va_start(ap, vector);
    bool status = vector_insert_internal(vector, 0, ap);
    va_end(ap);

    return status;
}

/*  Inserts a value into a vector at a specified index  */

bool vector_insert(struct vector * vector, const size_t index, ...)
{
    va_list ap;
    va_start(ap, index);
    bool status = vector_insert_internal(vector, index, ap);
    va_end(ap);

    return status;
}

/*  Deletes a value from a vector at a specified index  */

bool vector_delete_index(struct vector * vector, const size_t index)
{
    if ( index >= vector->length ) {
        if ( vector->exit_on_error ) {
            gds_error_quit("index %zu out of range (%s, line %d)",
                           index, __FILE__, __LINE__);
        }
        else {
            return false;
        }
    }

    gdt_free(&vector->elements[index], vector->type);

    if ( index != vector->length - 1 ) {
        struct gdt_generic_datatype * src = vector->elements + index + 1;
        struct gdt_generic_datatype * dst = src - 1;
        const size_t numcopy = vector->length - index - 1;
        memmove(dst, src, numcopy * sizeof *src);
    }

    vector->length -= 1;

    return true;
}

/*  Deletes the first element in a vector  */

bool vector_delete_front(struct vector * vector)
{
    return vector_delete_index(vector, 0);
}

/*  Deletes the last element in a vector  */

bool vector_delete_back(struct vector * vector)
{
    return vector_delete_index(vector, vector->length - 1);
}

/*  Gets the data at a specified index  */

bool vector_element_at_index(struct vector * vector,
                             const size_t index, void * p)
{
    if ( index >= vector->length ) {
        if ( vector->exit_on_error ) {
            gds_error_quit("index %zu out of range (%s, line %d)",
                           index, __FILE__, __LINE__);
        }
        else {
            return false;
        }
    }

    gdt_get_value(&vector->elements[index], vector->type, p);

    return true;
}

/*  Sets the data at a specified index  */

bool vector_set_element_at_index(struct vector * vector,
                                 const size_t index, ...)
{
    if ( index >= vector->length ) {
        if ( vector->exit_on_error ) {
            gds_error_quit("index %zu out of range (%s, line %d)",
                           index, __FILE__, __LINE__);
        }
        else {
            return false;
        }
    }

    va_list ap;
    va_start(ap, index);
    gdt_set_value(&vector->elements[index], vector->type, vector->compfunc, ap);
    va_end(ap);

    return true;
}

/*  Finds an element in a vector  */

bool vector_find(struct vector * vector, size_t * index, ...)
{
    struct gdt_generic_datatype needle;
    va_list ap;
    va_start(ap, index);
    gdt_set_value(&needle, vector->type, vector->compfunc, ap);
    va_end(ap);

    for ( size_t i = 0; i < vector->length; ++i ) {
        if ( !gdt_compare(&needle, &vector->elements[i]) ) {
            *index = i;
            return true;
        }
    }

    return false;
}

/*  Sorts the elements in a vector  */

void vector_sort(struct vector * vector)
{
    qsort(vector->elements, vector->length, sizeof *vector->elements,
          gdt_compare_void);
}

/*  Sorts the elements in a vector in reverse order  */

void vector_reverse_sort(struct vector * vector)
{
    qsort(vector->elements, vector->length, sizeof *vector->elements,
          gdt_reverse_compare_void);
}

/*  Checks if a vector is empty  */

bool vector_is_empty(struct vector * vector)
{
    return vector->length == 0;
}

/*  Returns the length of a vector  */

size_t vector_length(struct vector * vector)
{
    return vector->length;
}

/*  Returns the capacity of a vector  */

size_t vector_capacity(struct vector * vector)
{
    return vector->capacity;
}

/*  Returns the free space in a vector  */

size_t vector_free_space(struct vector * vector)
{
    return vector->capacity - vector->length;
}

/*  Inserts a node at a specified index  */

static bool vector_insert_internal(struct vector * vector,
                                   const size_t index, va_list ap)
{
    if ( index > vector->length ) {
        if ( vector->exit_on_error ) {
            gds_error_quit("index %zu out of range (%s, line %d)",
                           index, __FILE__, __LINE__);
        }
        else {
            return false;
        }
    }

    if ( vector->length == vector->capacity ) {
        const size_t new_capacity = vector->capacity * GROWTH;
        struct gdt_generic_datatype * new_elements;
        new_elements = realloc(vector->elements,
                               new_capacity * sizeof *new_elements);
        if ( !new_elements ) {
            return false;
        }
        vector->elements = new_elements;
        vector->capacity = new_capacity;
    }

    if ( index != vector->length ) {
        struct gdt_generic_datatype * src = vector->elements + index;
        struct gdt_generic_datatype * dst = src + 1;
        const size_t numcopy = vector->length - index;
        memmove(dst, src, numcopy * sizeof *src);
    }

    gdt_set_value(&vector->elements[index], vector->type, vector->compfunc, ap);
    vector->length += 1;

    return true;
}
