/*!
 * \file            gdt.h
 * \brief           Interface to generic data element functionality.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_GENERIC_DATATYPE_H
#define PG_GENERIC_DATA_STRUCTURES_GENERIC_DATATYPE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

#include "gds_public_types.h"

/*!
 * \brief           Generic datatype structure
 */
struct gdt_generic_datatype {
    enum gds_datatype type;             /*!<  Data type                     */
    gds_cfunc compfunc;                 /*!<  Comparison function pointer   */
    union {
        char c;                             /*!<  char                      */
        unsigned char uc;                   /*!<  unsigned char             */
        signed char sc;                     /*!<  signed char               */
        int i;                              /*!<  int                       */
        unsigned int ui;                    /*!<  unsigned int              */
        long l;                             /*!<  long                      */
        unsigned long ul;                   /*!<  unsigned long             */
        long long int ll;                   /*!<  long long                 */
        unsigned long long int ull;         /*!<  unsigned long long        */
        size_t st;                          /*!<  size_t                    */
        double d;                           /*!<  double                    */
        char * pc;                          /*!<  char *, string            */
        void * p;                           /*!<  void *                    */
    } data;                             /*!<  Data union                */
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

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GENERIC_DATATYPE_H  */
