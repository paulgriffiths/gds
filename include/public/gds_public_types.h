/*!
 * \file            gds_public_types.h
 * \brief           Common public types for generic data structures library
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_GENERIC_DATA_STRUCTURES_PUBLIC_TYPES_H
#define PG_GENERIC_DATA_STRUCTURES_PUBLIC_TYPES_H

#include <stdbool.h>
#include <stddef.h>

/*!
 *  \brief          Type definition for comparison function pointer.
 *  \ingroup        gdt
 */
typedef int (*gds_cfunc)(const void *, const void *);

/*!
 *  \brief          Enumeration type for data structure options.
 *  \ingroup        general
 */
enum gds_option {
    GDS_RESIZABLE = 1,          /*!<  Dynamically resizes on demand        */
    GDS_FREE_ON_DESTROY = 2,    /*!<  Automatically frees pointer members  */
    GDS_EXIT_ON_ERROR = 4       /*!<  Exits on error                       */
};

/*!
 *  \brief          Enumeration type for data element type.
 *  \ingroup        gdt
 */
enum gds_datatype {
    DATATYPE_CHAR,                  /*!<  char                */
    DATATYPE_UNSIGNED_CHAR,         /*!<  unsigned char       */
    DATATYPE_SIGNED_CHAR,           /*!<  signed char         */
    DATATYPE_INT,                   /*!<  int                 */
    DATATYPE_UNSIGNED_INT,          /*!<  unsigned int        */
    DATATYPE_LONG,                  /*!<  long                */
    DATATYPE_UNSIGNED_LONG,         /*!<  unsigned long       */
    DATATYPE_LONG_LONG,             /*!<  long long           */
    DATATYPE_UNSIGNED_LONG_LONG,    /*!<  unsigned long long  */
    DATATYPE_SIZE_T,                /*!<  size_t              */
    DATATYPE_DOUBLE,                /*!<  double              */
    DATATYPE_STRING,                /*!<  char *, string      */
    DATATYPE_POINTER                /*!<  void *              */
};

#endif      /*  PG_GENERIC_DATA_STRUCTURES_PUBLIC_TYPES_H  */
