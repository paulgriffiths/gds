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
 * \ingroup         gdt
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

/*!
 * \brief           Sets the value of a generic datatype.
 * \ingroup         gdt
 * \param data      A pointer to the generic datatype.
 * \param type      The type of data for the datatype to contain.
 * \param cfunc     A pointer to a comparison function. This is ignored
 * for all types other than `DATATYPE_POINTER`. For `DATATYPE_POINTER`, this
 * should contain the address of a function of type `int (*)(const void *,
 * const void *)` if the datatype will ever need to be compared with another
 * datatype of the same type (e.g. for finding or sorting elements within a
 * data structure). If this functionality is not required, `NULL` can be
 * provided.
 * \param ap        A `va_list` containing a single argument of the type
 * appropriate to `type`, containing the value to which to set the generic
 * datatype.
 */
void gdt_set_value(struct gdt_generic_datatype * data,
                   const enum gds_datatype type,
                   gds_cfunc cfunc, va_list ap);

/*!
 * \brief           Gets the value of a generic datatype.
 * \ingroup         gdt
 * \param data      A pointer to the generic datatype.
 * \param p         A pointer containing the address of an object of type
 * appropriate to the type of the generic datatype set by the last call to
 * `gdt_set_value()`. This object will be modified to contain the value
 * of the generic datatype.
 */
void gdt_get_value(const struct gdt_generic_datatype * data, void * p);

/*!
 * \brief           Frees memory pointed to by a generic datatype.
 * \ingroup         gdt
 * \details         This function does nothing if the type of the generic
 * datatype set by the last call to `gdt_set_value()` is neither
 * `DATATYPE_STRING` nor `DATATYPE_POINTER`. If the type of the generic
 * datatype is one of these values, the caller is responsible for ensuring
 * that the last value set contains an address on which it is appropriate
 * to call `free()`.
 * \param data      A pointer to the generic datatype.
 */
void gdt_free(struct gdt_generic_datatype * data);

/*!
 * \brief           Compares two generic datatypes.
 * \ingroup         gdt
 * \param d1        A pointer to the first generic datatype.
 * \param d2        A pointer to the second generic datatype.
 * \retval 0        The two datatypes are equal.
 * \retval -1       The first datatype is less than the second datatype.
 * \retval 1        The first datatype is greater than the second datatype.
 */
int gdt_compare(const struct gdt_generic_datatype * d1,
                const struct gdt_generic_datatype * d2);

/*!
 * \brief           Compares two generic datatypes via `void` pointers.
 * \ingroup         gdt
 * \details         This function is suitable for passing to `qsort()`.
 * \param p1        A pointer to the first generic datatype.
 * \param p2        A pointer to the second generic datatype.
 * \retval 0        The two datatypes are equal.
 * \retval -1       The first datatype is less than the second datatype.
 * \retval 1        The first datatype is greater than the second datatype.
 */
int gdt_compare_void(const void * p1, const void * p2);

/*!
 * \brief           Reverse compares two generic datatypes via `void` pointers.
 * \ingroup         gdt
 * \details         This function is suitable for passing to `qsort()`
 * when the desired behavior is to sort in reverse order.
 * \param p1        A pointer to the first generic datatype.
 * \param p2        A pointer to the second generic datatype.
 * \retval 0        The two datatypes are equal.
 * \retval -1       The first datatype is greater than the second datatype.
 * \retval 1        The first datatype is less than the second datatype.
 */
int gdt_reverse_compare_void(const void * p1, const void * p2);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_GENERIC_DATATYPE_H  */
