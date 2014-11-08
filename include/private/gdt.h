#ifndef PG_SAMPLES_AND_DEMOS_GENERIC_DATATYPE_H
#define PG_SAMPLES_AND_DEMOS_GENERIC_DATATYPE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

#include "gds_public_types.h"

struct gdt_generic_datatype {
    enum gds_datatype type;
    gds_cfunc compfunc;
    union {
        char c;
        unsigned char uc;
        signed char sc;
        int i;
        unsigned int ui;
        long l;
        unsigned long ul;
        long long int ll;
        unsigned long long int ull;
        size_t st;
        double d;
        char * pc;
        void * p;
    } data;
};

void gdt_set_value(struct gdt_generic_datatype * data,
                   const enum gds_datatype,
                   gds_cfunc cfunc, va_list ap);
void gdt_get_value(const struct gdt_generic_datatype * data,
                   const enum gds_datatype type, void * p);
void gdt_free(struct gdt_generic_datatype * data,
              const enum gds_datatype type);
int gdt_compare(const struct gdt_generic_datatype * d1,
                const struct gdt_generic_datatype * d2);
int gdt_compare_void(const void * p1, const void * p2);
int gdt_reverse_compare_void(const void * p1, const void * p2);

#endif      /*  PG_SAMPLES_AND_DEMOS_GENERIC_DATATYPE_H  */
