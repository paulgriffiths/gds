/*!
 * \file            dict.c
 * \brief           Implementation of generic dictionary data structure.
 * \todo            Finish implementing dictionary.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "gds_common.h"
#include "dict.h"
#include "list.h"

/*!  Number of buckets  */
static const size_t BUCKETS = 256;

/*!  Key-Value pair structure  */
typedef struct kvpair {
    char * key;                               /*!<  String key              */
    struct gdt_generic_datatype value;        /*!<  Generic datatype value  */
} * KVPair;

/*!  Dict structure  */
struct dict {
    size_t num_buckets;                         /*!<  Number of buckets     */
    List * buckets;                             /*!<  The buckets           */
    enum gds_datatype type;                     /*!<  Dict datatype         */
    bool free_on_destroy;   /*!<  Free pointer elements on destroy if true  */
    bool exit_on_error;     /*!<  Exit on error if true                     */
};

static KVPair kvpair_create(const char * key, 
                            const enum gds_datatype type,
                            va_list ap);
static KVPair kvpair_create_novalue(const char * key);
static void kvpair_destroy(KVPair kvpair, const bool free_value);
static int kvpair_compare(const void * p1, const void * p2);

static bool dict_has_key_internal(Dict dict, const char * key, KVPair * pair);

/*!
 * \brief           Calculates a hash of a string.
 * \details         Uses Dan Bernstein's djb2 algorithm.
 * \param str       A pointer to a string
 * \returns         The hash value
 */
static size_t djb2hash(const char * str);

Dict dict_create(const enum gds_datatype type, const int opts)
{
    struct dict * new_dict = malloc(sizeof *new_dict);
    if ( !new_dict ) {
        if ( opts & GDS_EXIT_ON_ERROR ) {
            gds_strerror_quit("memory allocation failed "
                              "(%s, line %d)", __FILE__, __LINE__);
        }
        else {
            return NULL;
        }
    }

    new_dict->num_buckets = BUCKETS;
    new_dict->type = type;
    new_dict->free_on_destroy = (opts & GDS_FREE_ON_DESTROY) ? true : false;
    new_dict->exit_on_error = (opts & GDS_EXIT_ON_ERROR) ? true : false;

    new_dict->buckets = malloc(sizeof *new_dict->buckets *
                               new_dict->num_buckets);
    if ( !new_dict->buckets ) {
        if ( new_dict->exit_on_error ) {
            gds_strerror_quit("memory allocation failed "
                              "(%s, line %d)", __FILE__, __LINE__);
        }
        else {
            free(new_dict);
            return NULL;
        }
    }

    size_t i = 0;
    for ( i = 0; i < new_dict->num_buckets; ++i ) {
        new_dict->buckets[i] = list_create(DATATYPE_POINTER,
                                           0,
                                           kvpair_compare);
        if ( !new_dict->buckets[i] ) {
            if ( new_dict->exit_on_error ) {
                gds_strerror_quit("memory allocation failed "
                                  "(%s, line %d)", __FILE__, __LINE__);
            }
            else {
                break;
            }
        }
    }

    if ( i < new_dict->num_buckets ) {

        /*  Not all buckets were successfully created, so
         *  free the ones we did make, and return with error  */

        for ( size_t j = 0; j < i; ++j ) {
            list_destroy(new_dict->buckets[j]);
        }

        free(new_dict->buckets);
        free(new_dict);
        return NULL;
    }
                                           
    return new_dict;
}

void dict_destroy(Dict dict)
{
    for ( size_t i = 0; i < dict->num_buckets; ++i ) {
        struct kvpair * pair;
        List list = dict->buckets[i];
        while ( list_element_at_index(list, 0, (void *) &pair) ) {
            list_delete_front(list);
            kvpair_destroy(pair, dict->free_on_destroy);
        }
        list_destroy(dict->buckets[i]);
    }
    free(dict->buckets);
    free(dict);
}

int dict_has_key(Dict dict, const char * key)
{
    return dict_has_key_internal(dict, key, NULL);
}

bool dict_insert(Dict dict, const char * key, ...)
{
    struct kvpair * pair;
    int status = dict_has_key_internal(dict, key, &pair);
    if ( status == -1 ) {
        return false;
    }
    else if ( status ) {
        va_list ap;
        va_start(ap, key);
        gdt_set_value(&pair->value, dict->type, NULL, ap);
        va_end(ap);
    }
    else {
        va_list ap;
        va_start(ap, key);
        struct kvpair * new_pair = kvpair_create(key, dict->type, ap);
        va_end(ap);

        if ( !new_pair ) {
            return false;
        }

        const size_t hash = djb2hash(key) % dict->num_buckets;
        List list = dict->buckets[hash];
        if ( !list_append(list, (void *) new_pair) ) {
            kvpair_destroy(new_pair, dict->free_on_destroy);
            return false;
        }
    }

    return true;
}

static bool dict_has_key_internal(Dict dict, const char * key, KVPair * pair)
{
    struct kvpair needle = { .key = (char *) key };
    const size_t hash = djb2hash(key) % dict->num_buckets;
    List list = dict->buckets[hash];
    size_t index;

    if ( list_find(list, &index, (void *) &needle) ) {
        if ( pair ) {
            list_element_at_index(list, index, (void *) pair);
        }

        return true;
    }
    else {
        if ( pair ) {
            *pair = NULL;
        }

        return false;
    }
}

static KVPair kvpair_create(const char * key, 
                            const enum gds_datatype type,
                            va_list ap)
{
    struct kvpair * new_pair = kvpair_create_novalue(key);
    if ( !new_pair ) {
        return NULL;
    }

    gdt_set_value(&new_pair->value, type, NULL, ap);

    return new_pair;
}

static KVPair kvpair_create_novalue(const char * key)
{
    struct kvpair * new_pair = malloc(sizeof *new_pair);
    if ( !new_pair ) {
        return NULL;
    }

    new_pair->key = gds_strdup(key);
    if ( !new_pair->key ) {
        free(new_pair);
        return NULL;
    }

    return new_pair;
}

static int kvpair_compare(const void * p1, const void * p2)
{
    const struct kvpair * kv1 = *((const void **) p1);
    const struct kvpair * kv2 = *((const void **) p2);
    return strcmp(kv1->key, kv2->key);
}

static void kvpair_destroy(KVPair kvpair, const bool free_value)
{
    free(kvpair->key);
    if ( free_value ) {
        gdt_free(&kvpair->value);
    }
    free(kvpair);
}

static size_t djb2hash(const char * str)
{
    size_t hash = 5381;
    int c;

    while ( (c = *str++) ) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}
