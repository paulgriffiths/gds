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
#include <stdarg.h>
#include "gds_common.h"
#include "dict.h"
#include "list.h"

/*!  Number of buckets  */
static const size_t BUCKETS = 256;

struct KVPair {
    char * key;
    struct gds_generic_datatype value;
};

/*!  Dict structure  */
struct dict {
    size_t num_buckets;                         /*!<  Number of buckets    */
    List * buckets;                             /*!<  The buckets          */
    enum gds_datatype type;                     /*!<  Dict datatype        */
    bool free_on_destroy;   /*!<  Free pointer elements on destroy if true  */
    bool exit_on_error;     /*!<  Exit on error if true                     */
};

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

    new_dict->buckets = malloc(sizeof *new_dict->elements *
                               new_dict->num_buckets);
    if ( !new_dict->elements ) {
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
        new_dict->buckets[i] = list_create(DATATYPE_POINTER, opts, NULL);
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
        for ( size_t j = 0; j <= i; ++j ) {
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
        list_destroy(dict->buckets[i]);
    }
    free(dict->buckets);
    free(dict);
}
