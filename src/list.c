#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "gds_common.h"
#include "list.h"

static const size_t GROWTH = 2;

struct list_node {
    struct gdt_generic_datatype element;
    struct list_node * prev;
    struct list_node * next;
};

struct list {
    size_t length;
    enum gds_datatype type;
    gds_cfunc compfunc;
    struct list_node * head;
    struct list_node * tail;

    bool free_on_destroy;
    bool exit_on_error;
};

static struct list_node * list_node_create(struct list * list, va_list ap);
static void list_node_destroy(struct list * list, struct list_node * node);

static struct list_node * list_node_at_index(struct list * list,
                                             const size_t index);
static bool list_insert_internal(struct list * list,
                                 struct list_node * node,
                                 const size_t index);

/*  Creates and returns a new list of specified type  */

struct list * list_create(const enum gds_datatype type,
                          const int opts, ...)
{
    struct list * new_list = malloc(sizeof *new_list);
    if ( !new_list ) {
        if ( opts & GDS_EXIT_ON_ERROR ) {
            gds_strerror_quit("memory allocation failed "
                              "(%s, line %d)", __FILE__, __LINE__);
        }
        else {
            return NULL;
        }
    }

    new_list->length = 0;
    new_list->type = type;
    new_list->head = NULL;
    new_list->tail = NULL;

    new_list->free_on_destroy = (opts & GDS_FREE_ON_DESTROY) ? true : false;
    new_list->exit_on_error = (opts & GDS_EXIT_ON_ERROR) ? true : false;

    va_list ap;
    va_start(ap, opts);
    if ( type == DATATYPE_POINTER ) {
        new_list->compfunc = va_arg(ap, int (*)(const void *, const void *));
    }
    else {
        new_list->compfunc = NULL;
    }
    va_end(ap);

    return new_list;
}

/*  Destroys a previously created list  */

void list_destroy(struct list * list)
{
    while ( !list_is_empty(list) ) {
        list_delete_front(list);
    }

    free(list);
}

/*  Appends a value to the end of a list  */

bool list_append(struct list * list, ...)
{
    va_list ap;
    va_start(ap, list);
    struct list_node * new_node = list_node_create(list, ap);
    va_end(ap);

    return list_insert_internal(list, new_node, list->length);
}

/*  Prepends a value to the front of a list  */

bool list_prepend(struct list * list, ...)
{
    va_list ap;
    va_start(ap, list);
    struct list_node * new_node = list_node_create(list, ap);
    va_end(ap);

    return list_insert_internal(list, new_node, 0);
}

/*  Inserts a value into a list at a specified index  */

bool list_insert(struct list * list, const size_t index, ...)
{
    va_list ap;
    va_start(ap, index);
    struct list_node * new_node = list_node_create(list, ap);
    va_end(ap);

    return list_insert_internal(list, new_node, index);
}

/*  Deletes a value from a list at a specified index  */

bool list_delete_index(struct list * list, const size_t index)
{
    struct list_node * dead = list_node_at_index(list, index);
    if ( !dead ) {
        return false;
    }

    if ( list->length == 1 ) {
        list->head = NULL;
        list->tail = NULL;
    }
    else if ( index == 0 ) {
        struct list_node * new_head = list->head->next;
        new_head->prev = NULL;
        list->head = new_head;
    }
    else if ( index == list->length - 1 ) {
        struct list_node * new_tail = list->tail->prev;
        new_tail->next = NULL;
        list->tail = new_tail;
    }
    else {
        struct list_node * before = dead->prev;
        struct list_node * after = dead->next;
        before->next = after;
        after->prev = before;
    }

    list->length -= 1;
    list_node_destroy(list, dead);

    return true;
}

/*  Deletes the first element in a list  */

bool list_delete_front(struct list * list)
{
    return list_delete_index(list, 0);
}

/*  Deletes the last element in a list  */

bool list_delete_back(struct list * list)
{
    return list_delete_index(list, list->length - 1);
}

/*  Gets the data at a specified index  */

bool list_element_at_index(struct list * list, const size_t index, void * p)
{
    struct list_node * node = list_node_at_index(list, index);
    if ( !node ) {
        return false;
    }

    gdt_get_value(&node->element, list->type, p);

    return true;
}

/*  Sets the data at a specified index  */

bool list_set_element_at_index(struct list * list, const size_t index, ...)
{
    struct list_node * node = list_node_at_index(list, index);
    if ( !node ) {
        return false;
    }

    va_list ap;
    va_start(ap, index);
    gdt_set_value(&node->element, list->type, list->compfunc, ap);
    va_end(ap);

    return true;
}

/*  Finds an element in a list  */

bool list_find(struct list * list, size_t * index, ...)
{
    struct gdt_generic_datatype needle;
    va_list ap;
    va_start(ap, index);
    gdt_set_value(&needle, list->type, list->compfunc, ap);
    va_end(ap);

    struct list_node * node = list->head;
    size_t i = 0;
    while ( node ) {
        if ( !gdt_compare(&needle, &node->element) ) {
            *index = i;
            return true;
        }

        node = node->next;
        ++i;
    }

    return false;
}

/*  Checks if a list is empty  */

bool list_is_empty(struct list * list)
{
    return list->length == 0;
}

/*  Returns the length of a list  */

size_t list_length(struct list * list)
{
    return list->length;
}

/*  Creates a new list node  */

static struct list_node * list_node_create(struct list * list, va_list ap)
{
    struct list_node * new_node = malloc(sizeof *new_node);
    if ( !new_node ) {
        if ( list->exit_on_error ) {
            gds_strerror_quit("memory allocation failed "
                              "(%s, line %d)", __FILE__, __LINE__);
        }
        else {
            return NULL;
        }
    }

    new_node->prev = NULL;
    new_node->next = NULL;
    gdt_set_value(&new_node->element, list->type, list->compfunc, ap);

    return new_node;
}

/*  Destroys a list node  */

static void list_node_destroy(struct list * list, struct list_node * node)
{
    if ( list->free_on_destroy ) {
        gdt_free(&node->element, list->type);
    }

    free(node);
}

/*  Returns the node at the specified index  */

static struct list_node * list_node_at_index(struct list * list,
                                             const size_t index)
{
    if ( index >= list->length ) {
        if ( list->exit_on_error ) {
            gds_error_quit("index %zu out of range (%s, line %d)",
                           index, __FILE__, __LINE__);
        }
        else {
            return NULL;
        }
    }

    struct list_node * node;
    if ( index == 0 ) {
        node = list->head;
    }
    else if ( index == list->length - 1 ) {
        node = list->tail;
    }
    else {
        if ( index < list->length / 2 ) {
            node = list->head;
            for ( size_t i = 0; i < index; ++i ) {
                node = node->next;
            }
        }
        else {
            node = list->tail;
            for ( size_t i = list->length - 1; i > index; --i ) {
                node = node->prev;
            }
        }
    }

    return node;
}

/*  Inserts a node at a specified index  */

static bool list_insert_internal(struct list * list,
                                 struct list_node * node,
                                 const size_t index)
{
    if ( !node ) {
        return false;
    }
    else if ( index > list->length ) {
        if ( list->exit_on_error ) {
            gds_error_quit("index %zu out of range (%s, line %d)",
                           index, __FILE__, __LINE__);
        }
        else {
            free(node);
            return false;
        }
    }

    if ( !list->head ) {
        list->head = node;
        list->tail = node;
    }
    else if ( index == 0 ) {
        struct list_node * old_head = list->head;
        node->next = old_head;
        old_head->prev = node;
        list->head = node;
    }
    else if ( index == list->length ) {
        struct list_node * old_tail = list->tail;
        node->prev = old_tail;
        old_tail->next = node;
        list->tail = node;
    }
    else {
        struct list_node * after = list_node_at_index(list, index);
        struct list_node * before = after->prev;
        node->prev = before;
        node->next = after;
        before->next = node;
        after->prev = node;
    }

    list->length += 1;

    return true;
}
