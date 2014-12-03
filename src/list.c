/*!
 * \file            list.c
 * \brief           Implementation of generic list data structure.
 * \details         The list is implemented as a double-ended, double-linked
 * list.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pggds_internal/gds_common.h>
#include <pggds/list.h>

/*!  List node structure  */
typedef struct list_node {
    struct gdt_generic_datatype element;    /*!<  Data element              */
    struct list_node * prev;                /*!<  Pointer to previous node  */
    struct list_node * next;                /*!<  Pointer to next node      */
    struct list * list;                     /*!<  Pointer to owning list    */
} * ListNode;

/*!  List structure  */
struct list {
    size_t length;              /*!<  Length of list                        */
    enum gds_datatype type;     /*!<  List datatype                         */
    gds_cfunc compfunc;         /*!<  Element comparison function           */
    struct list_node * head;    /*!<  Pointer to head of list               */
    struct list_node * tail;    /*!<  Pointer to tail of list               */

    bool free_on_destroy;   /*!<  Free pointer elements on destroy if true  */
    bool exit_on_error;     /*!<  Exit on error if true                     */
};

/*!
 * \brief           Private function to create list node.
 * \param list      A pointer to the list.
 * \param ap        A `va_list` containing the data value for the node.
 * This should be of a type appropriate to the type set when creating
 * the list.
 * \retval NULL     Failure, dynamic memory allocation failed
 * \retval non-NULL A pointer to the new node
 */
static ListNode list_node_create(List list, va_list ap);

/*!
 * \brief           Destroys a list node.
 * \details         If the `GDS_FREE_ON_DESTROY` option was specified
 * when creating the list, any pointer values still in the list will
 * be `free()`d prior to destruction.
 * \param list      A pointer to the list.
 * \param node      A pointer to the node.
 */
static void list_node_destroy(List list, ListNode node);

/*!
 * \brief           Private function to return the node at a specified index.
 * \param list      A pointer to the list.
 * \param index     The index of the requested node.
 * \retval NULL     Failure, index out of range
 * \retval non-NULL A pointer to the node at the specified index
 */
static ListNode list_node_at_index(List list, const size_t index);

/*!
 * \brief           Private function to insert a node before another.
 * \param list      A pointer to the list.
 * \param itr       The node before which to insert.
 * \param new_node  The node to insert.
 */
static void list_insert_before_itr_internal(List list,
                                            ListItr itr,
                                            ListItr new_node);

/*!
 * \brief           Private function to insert a node after another.
 * \param list      A pointer to the list.
 * \param itr       The node after which to insert.
 * \param new_node  The node to insert.
 */
static void list_insert_after_itr_internal(List list,
                                           ListItr itr,
                                           ListItr new_node);

/*!
 * \brief           Private function to insert a list with compare function.
 * \details         The sort and reverse sort function differ only in the
 * compare function used. This private function packages up all the other
 * logic into a single function for reuse.
 * \param list      A pointer to the list.
 * \param compfunc  A pointer to the compare function to use.
 * \retval true     Success
 * \retval false    Failure, memory allocation failed.
 */
static bool list_sort_internal(List list, gds_cfunc compfunc);

List list_create(const enum gds_datatype type, const int opts, ...)
{
    struct list * new_list = malloc(sizeof *new_list);
    if ( !new_list ) {
        if ( opts & GDS_EXIT_ON_ERROR ) {
            quit_strerror("gds library", "memory allocation failed");
        }
        else {
            log_strerror("gds library", "memory allocation failed");
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

        /*  Custom comparison function only needed for void * members  */

        new_list->compfunc = va_arg(ap, int (*)(const void *, const void *));
    }
    else {
        new_list->compfunc = NULL;
    }
    va_end(ap);

    return new_list;
}

void list_destroy(List list)
{
    while ( list_delete_itr(list->head) ) {  /*  Empty  */  }
    free(list);
}

bool list_append(List list, ...)
{
    va_list ap;
    va_start(ap, list);
    struct list_node * new_node = list_node_create(list, ap);
    va_end(ap);

    if ( new_node ) {
        list_insert_after_itr_internal(list, list->tail, new_node);
        return true;
    }

    return false;
}

bool list_prepend(List list, ...)
{
    va_list ap;
    va_start(ap, list);
    struct list_node * new_node = list_node_create(list, ap);
    va_end(ap);

    if ( new_node ) {
        list_insert_before_itr_internal(list, list->head, new_node);
        return true;
    }

    return false;
}

bool list_insert(List list, const size_t index, ...)
{
    va_list ap;
    va_start(ap, index);
    struct list_node * new_node = list_node_create(list, ap);
    va_end(ap);

    if ( new_node ) {
        if ( index == 0 ) {
            list_insert_before_itr_internal(list, list->head, new_node);
        }
        else if ( index == list->length ) {
            list_insert_after_itr_internal(list, list->tail, new_node);
        }
        else {
            struct list_node * dead = list_node_at_index(list, index);
            list_insert_before_itr_internal(list, dead, new_node);
        }

        return true;
    }

    return false;
}

bool list_delete_index(List list, const size_t index)
{
    struct list_node * dead = list_node_at_index(list, index);
    if ( !dead ) {

        /*  Index out of range, list possibly empty.  */

        return false;
    }

    list_delete_itr(dead);
    return true;
}

bool list_delete_front(List list)
{
    if ( list->head ) {
        list_delete_itr(list->head);
        return true;
    }
    else {
        return false;
    }
}

bool list_delete_back(List list)
{
    if ( list->tail ) {
        list_delete_itr(list->tail);
        return true;
    }
    else {
        return false;
    }
}

bool list_element_at_index(List list, const size_t index, void * p)
{
    struct list_node * node = list_node_at_index(list, index);
    if ( !node ) {

        /*  Index out of range, list possible empty  */

        return false;
    }

    gdt_get_value(&node->element, p);

    return true;
}

bool list_set_element_at_index(List list, const size_t index, ...)
{
    struct list_node * node = list_node_at_index(list, index);
    if ( !node ) {

        /*  Index out of range, list possibly empty  */

        return false;
    }

    va_list ap;
    va_start(ap, index);
    gdt_set_value(&node->element, list->type, list->compfunc, ap);
    va_end(ap);

    return true;
}

bool list_find(List list, size_t * index, ...)
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

ListItr list_find_itr(List list, ...)
{
    struct gdt_generic_datatype needle;
    va_list ap;
    va_start(ap, list);
    gdt_set_value(&needle, list->type, list->compfunc, ap);
    va_end(ap);

    struct list_node * node = list->head;
    while ( node ) {
        if ( !gdt_compare(&needle, &node->element) ) {
            return node;
        }
        node = node->next;
    }

    return NULL;
}

bool list_sort(List list)
{
    return list_sort_internal(list, gdt_compare_void);
}

bool list_reverse_sort(List list)
{
    return list_sort_internal(list, gdt_reverse_compare_void);
}

ListItr list_itr_first(List list)
{
    return list->head;
}

ListItr list_itr_last(List list)
{
    return list->tail;
}

ListItr list_itr_next(ListItr itr)
{
    return itr->next;
}

ListItr list_itr_previous(ListItr itr)
{
    return itr->prev;
}

void list_get_value_itr(ListItr itr, void * p)
{
    gdt_get_value(&itr->element, p);
}

bool list_insert_before_itr(ListItr itr, ...)
{
    if ( itr ) {
        va_list ap;
        va_start(ap, itr);
        struct list_node * new_node = list_node_create(itr->list, ap);
        va_end(ap);

        if ( new_node ) {
            list_insert_before_itr_internal(itr->list, itr, new_node);
            return true;
        }
    }

    return false;
}

bool list_insert_after_itr(ListItr itr, ...)
{
    if ( itr ) {
        va_list ap;
        va_start(ap, itr);
        struct list_node * new_node = list_node_create(itr->list, ap);
        va_end(ap);

        if ( new_node ) {
            list_insert_after_itr_internal(itr->list, itr, new_node);
            return true;
        }
    }

    return false;
}

ListItr list_delete_itr(ListItr itr)
{
    if ( itr ) {
        ListItr next = itr->next;
        ListItr prev = itr->prev;
        List list = itr->list;

        if ( itr == list->head && itr == list->tail ) {
            list->head = NULL;
            list->tail = NULL;
        }
        else if ( itr == list->head ) {
            list->head = next;
            next->prev = NULL;
        }
        else if ( itr == list->tail ) {
            list->tail = prev;
            prev->next = NULL;
        }
        else {
            prev->next = next;
            next->prev = prev;
        }

        list->length -= 1;
        list_node_destroy(list, itr);
        return next;
    }
    else {
        return NULL;
    }
}

bool list_is_empty(List list)
{
    return list->length == 0;
}

size_t list_length(List list)
{
    return list->length;
}

static ListNode list_node_create(List list, va_list ap)
{
    struct list_node * new_node = malloc(sizeof *new_node);
    if ( !new_node ) {
        if ( list->exit_on_error ) {
            quit_strerror("gds library", "memory allocation failed");
        }
        else {
            log_strerror("gds library", "memory allocation failed");
            return NULL;
        }
    }

    new_node->prev = NULL;
    new_node->next = NULL;
    new_node->list = list;
    gdt_set_value(&new_node->element, list->type, list->compfunc, ap);

    return new_node;
}

static void list_node_destroy(List list, ListNode node)
{
    if ( list->free_on_destroy ) {
        gdt_free(&node->element);
    }

    free(node);
}

static ListNode list_node_at_index(List list, const size_t index)
{
    if ( index >= list->length ) {
        if ( list->exit_on_error ) {
            quit_error("gds library", "index %zu out of range", index);
        }
        else {
            log_error("gds library", "index %zu out of range", index);
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
            
            /*  For efficiency, search forward from the head if the
             *  specified index is closer to the head than the tail...  */

            node = list->head;
            for ( size_t i = 0; i < index; ++i ) {
                node = node->next;
            }
        }
        else {

            /*  ...and search backward from the tail if it's not.  */

            node = list->tail;
            for ( size_t i = list->length - 1; i > index; --i ) {
                node = node->prev;
            }
        }
    }

    return node;
}

static void list_insert_before_itr_internal(List list,
                                            ListItr itr,
                                            ListItr new_node)
{
    gds_assert(new_node, "gds library", "new_node parameter was NULL");

    if ( !itr ) {
        gds_assert(list->length == 0, "gds library",
                                      "list length was not zero");
        list->head = list->tail = new_node;
    }
    else if ( itr == list->head ) {
        new_node->next = list->head;
        list->head->prev = new_node;
        list->head = new_node;
    }
    else {
        new_node->prev = itr->prev;
        new_node->next = itr;
        itr->prev->next = new_node;
        itr->prev = new_node;
    }

    list->length += 1;
}

static void list_insert_after_itr_internal(List list,
                                           ListItr itr,
                                           ListItr new_node)
{
    gds_assert(new_node, "gds library", "new_node parameter was NULL");

    if ( !itr ) {
        gds_assert(list->length == 0, "gds library",
                                      "list length was not zero");
        list->head = list->tail = new_node;
    }
    else if ( itr == list->tail ) {
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
    else {
        new_node->prev = itr;
        new_node->next = itr->next;
        itr->next->prev = new_node;
        itr->next = new_node;
    }

    list->length += 1;
}

static bool list_sort_internal(List list, gds_cfunc compfunc)
{
    if ( list->length < 2 ) {

        /*  No point sorting an empty or one-element list.  */

        return true;
    }

    /*  Create temporary array  */

    struct gdt_generic_datatype * tempelem;
    tempelem = malloc(list->length * sizeof *tempelem);
    if ( !tempelem ) {
        if ( list->exit_on_error ) {
            quit_strerror("gds library", "memory allocation failed");
        }
        else {
            log_strerror("gds library", "memory allocation failed");
            return false;
        }
    }

    /*  Copy list elements to array  */

    struct list_node * node = list->head;
    size_t index = 0;
    while ( node ) {
        memcpy(tempelem + index, &node->element, sizeof *tempelem);
        node = node->next;
        ++index;
    }

    /*  Sort array  */

    qsort(tempelem, list->length, sizeof *tempelem, compfunc);

    /*  Copy array elements back to list  */

    index = 0;
    node = list->head;
    while ( node ) {
        memcpy(&node->element, tempelem + index, sizeof *tempelem);
        node = node->next;
        ++index;
    }

    free(tempelem);

    return true;
}
