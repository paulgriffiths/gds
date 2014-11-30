/*!
 * \file            kvpair.c
 * \brief           Implementation of generic key-value pair structure.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pggds_internal/gds_common.h>
#include <pggds/gds_util.h>
#include <pggds/kvpair.h>

KVPair gds_kvpair_create(const char * key,
                                const enum gds_datatype type,
                                va_list ap)
{
    struct gds_kvpair * new_pair = malloc(sizeof *new_pair);
    if ( !new_pair ) {
        log_strerror("gds library", "memory allocation failed");
        return NULL;
    }

    new_pair->key = gds_strdup(key);
    if ( !new_pair->key ) {
        log_strerror("gds library", "memory allocation failed");
        free(new_pair);
        return NULL;
    }

    gdt_set_value(&new_pair->value, type, NULL, ap);

    return new_pair;
}

void gds_kvpair_destroy(KVPair pair, const bool free_value)
{
    free(pair->key);
    if ( free_value ) {
        gdt_free(&pair->value);
    }
    free(pair);
}

int gds_kvpair_compare(const void * p1, const void * p2)
{
    const struct gds_kvpair * kv1 = *((const void **) p1);
    const struct gds_kvpair * kv2 = *((const void **) p2);
    return strcmp(kv1->key, kv2->key);
}
