/*!
 * \file            dict.c
 * \brief           Implementation of generic dictionary data structure.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pggds_internal/gds_common.h>
#include <pggds/gds_util.h>
#include <pggds/dict.h>
#include <pggds/list.h>
#include <pggds/kvpair.h>

/*!  Number of buckets  */
static const size_t BUCKETS = 256;

/*!  Dict structure  */
struct dict {
    size_t num_buckets;                         /*!<  Number of buckets     */
    List * buckets;                             /*!<  The buckets           */
    enum gds_datatype type;                     /*!<  Dict datatype         */
    bool free_on_destroy;   /*!<  Free pointer elements on destroy if true  */
    bool exit_on_error;     /*!<  Exit on error if true                     */
};

/*!
 * \brief               Internal function to check for the existence of a key.
 * \details             If the key is present, `pair` will be modified to
 * contain the address of the key-value pair containing it.
 * \param dict          A pointer to the dictionary.
 * \param key           The key for which to search.
 * \param pitr          A pointer to a list iterator. If the key is found,
 * the iterator at this address will be modified to point to the list
 * element containing the corresponding key-value pair.
 * \retval true         Key was found
 * \retval false        Key was not found
 */
static bool dict_has_key_internal(Dict dict, const char * key, ListItr * pitr);

/*!
 * \brief               Helper function to create the dictionary buckets.
 * \param dict          A pointer to the dictionary.
 * \retval true         Success
 * \retval false        Failure, dynamic memory allocation failed.
 */
static bool dict_buckets_create(Dict dict);

/*!
 * \brief               Helper function to destroy the dictionary buckets.
 * \param dict          A pointer to the dictionary.
 */
static void dict_buckets_destroy(Dict dict);

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
            quit_strerror("gds library", "memory allocation failed");
        }
        else {
            log_strerror("gds library", "memory allocation failed");
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
            quit_strerror("gds library", "memory allocation failed");
        }
        else {
            log_strerror("gds library", "memory allocation failed");
            free(new_dict);
            return NULL;
        }
    }

    if ( !dict_buckets_create(new_dict) ) {
        dict_buckets_destroy(new_dict);
        free(new_dict->buckets);
        free(new_dict);
        return NULL;
    }
                                           
    return new_dict;
}

void dict_destroy(Dict dict)
{
    dict_buckets_destroy(dict);
    free(dict->buckets);
    free(dict);
}

bool dict_has_key(Dict dict, const char * key)
{
    return dict_has_key_internal(dict, key, NULL);
}

bool dict_insert(Dict dict, const char * key, ...)
{
    ListItr itr;
    if ( dict_has_key_internal(dict, key, &itr) ) {
        struct gds_kvpair * pair;
        list_get_value_itr(itr, &pair);

        if ( dict->free_on_destroy ) {

            /*  Free existing item if necessary  */

            gdt_free(&pair->value);
        }

        va_list ap;
        va_start(ap, key);
        gdt_set_value(&pair->value, dict->type, NULL, ap);
        va_end(ap);
    }
    else {
        va_list ap;
        va_start(ap, key);
        struct gds_kvpair * new_pair = gds_kvpair_create(key, dict->type, ap);
        va_end(ap);

        if ( !new_pair ) {
            return false;
        }

        const size_t hash = djb2hash(key) % dict->num_buckets;
        if ( !list_append(dict->buckets[hash], (void *) new_pair) ) {
            gds_kvpair_destroy(new_pair, dict->free_on_destroy);
            return false;
        }
    }

    return true;
}

bool dict_value_for_key(Dict dict, const char * key, void * p)
{
    struct gds_kvpair * pair;
    ListItr itr;
    if ( !dict_has_key_internal(dict, key, &itr) ) {
        return false;
    }

    list_get_value_itr(itr, &pair);
    gdt_get_value(&pair->value, p);

    return true;
}

bool dict_delete(Dict dict, const char * key)
{
    ListItr itr;
    if ( dict_has_key_internal(dict, key, &itr) ) {
        KVPair pair;
        list_get_value_itr(itr, &pair);
        gds_kvpair_destroy(pair, dict->free_on_destroy);
        list_delete_itr(itr);
        return true;
    }
    return false;
}

static bool dict_buckets_create(Dict dict)
{
    for ( size_t i = 0; i < dict->num_buckets; ++i ) {
        dict->buckets[i] = list_create(DATATYPE_POINTER, 0, gds_kvpair_compare);
        if ( !dict->buckets[i] ) {
            if ( dict->exit_on_error ) {
                quit_strerror("gds library", "memory allocation failed");
            }
            else {
                log_strerror("gds library", "memory allocation failed");
                return false;
            }
        }
    }

    return true;
}

static void dict_buckets_destroy(Dict dict)
{
    /*  If dict_buckets_create() failed, then the first pointer
     *  in dict->buckets[] corresponding to a failed list
     *  creation is set to NULL. Therefore loop over the total
     *  number of buckets, or until we encounter NULL, whichever
     *  comes first.                                              */

    for ( size_t i = 0; i < dict->num_buckets && dict->buckets[i]; ++i ) {
        ListItr itr = list_itr_first(dict->buckets[i]);
        while ( itr ) {
            struct gds_kvpair * pair;
            list_get_value_itr(itr, &pair);
            gds_kvpair_destroy(pair, dict->free_on_destroy);
            itr = list_itr_next(itr);
        }
        list_destroy(dict->buckets[i]);
    }
}

static bool dict_has_key_internal(Dict dict, const char * key, ListItr * pitr)
{
    struct gds_kvpair needle = { .key = (char *) key };
    const size_t hash = djb2hash(key) % dict->num_buckets;
    List list = dict->buckets[hash];
    ListItr itr;

    if ( (itr = list_find_itr(list, (void *) &needle)) ) {
        if ( pitr ) {
            *pitr = itr;
        }
        return true;
    }
    else {
        if ( pitr ) {
            *pitr = NULL;
        }
        return false;
    }
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
