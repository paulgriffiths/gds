/*!
 * \file            gdt.c
 * \brief           Implementation of generic data element functionality.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdarg.h>
#include <pggds_internal/gds_common.h>

/*!
 * \brief           Compare function for char
 * \param p1        Pointer to first value
 * \param p2        Pointer to second value
 * \retval 0        First value is equal to second value
 * \retval -1       First value is less than second value
 * \retval 1        First value is greater than second value
 */
static int gdt_compare_char(const void * p1, const void * p2);

/*!
 * \brief           Compare function for unsigned char
 * \param p1        Pointer to first value
 * \param p2        Pointer to second value
 * \retval 0        First value is equal to second value
 * \retval -1       First value is less than second value
 * \retval 1        First value is greater than second value
 */
static int gdt_compare_uchar(const void * p1, const void * p2);

/*!
 * \brief           Compare function for signed char
 * \param p1        Pointer to first value
 * \param p2        Pointer to second value
 * \retval 0        First value is equal to second value
 * \retval -1       First value is less than second value
 * \retval 1        First value is greater than second value
 */
static int gdt_compare_schar(const void * p1, const void * p2);

/*!
 * \brief           Compare function for int
 * \param p1        Pointer to first value
 * \param p2        Pointer to second value
 * \retval 0        First value is equal to second value
 * \retval -1       First value is less than second value
 * \retval 1        First value is greater than second value
 */
static int gdt_compare_int(const void * p1, const void * p2);

/*!
 * \brief           Compare function for unsigned int
 * \param p1        Pointer to first value
 * \param p2        Pointer to second value
 * \retval 0        First value is equal to second value
 * \retval -1       First value is less than second value
 * \retval 1        First value is greater than second value
 */
static int gdt_compare_uint(const void * p1, const void * p2);

/*!
 * \brief           Compare function for long
 * \param p1        Pointer to first value
 * \param p2        Pointer to second value
 * \retval 0        First value is equal to second value
 * \retval -1       First value is less than second value
 * \retval 1        First value is greater than second value
 */
static int gdt_compare_long(const void * p1, const void * p2);

/*!
 * \brief           Compare function for unsigned long
 * \param p1        Pointer to first value
 * \param p2        Pointer to second value
 * \retval 0        First value is equal to second value
 * \retval -1       First value is less than second value
 * \retval 1        First value is greater than second value
 */
static int gdt_compare_ulong(const void * p1, const void * p2);

/*!
 * \brief           Compare function for long long
 * \param p1        Pointer to first value
 * \param p2        Pointer to second value
 * \retval 0        First value is equal to second value
 * \retval -1       First value is less than second value
 * \retval 1        First value is greater than second value
 */
static int gdt_compare_longlong(const void * p1, const void * p2);

/*!
 * \brief           Compare function for unsigned long long
 * \param p1        Pointer to first value
 * \param p2        Pointer to second value
 * \retval 0        First value is equal to second value
 * \retval -1       First value is less than second value
 * \retval 1        First value is greater than second value
 */
static int gdt_compare_ulonglong(const void * p1, const void * p2);

/*!
 * \brief           Compare function for size_t
 * \param p1        Pointer to first value
 * \param p2        Pointer to second value
 * \retval 0        First value is equal to second value
 * \retval -1       First value is less than second value
 * \retval 1        First value is greater than second value
 */
static int gdt_compare_sizet(const void * p1, const void * p2);

/*!
 * \brief           Compare function for double
 * \param p1        Pointer to first value
 * \param p2        Pointer to second value
 * \retval 0        First value is equal to second value
 * \retval -1       First value is less than second value
 * \retval 1        First value is greater than second value
 */
static int gdt_compare_double(const void * p1, const void * p2);

/*!
 * \brief           Compare function for string
 * \param p1        Pointer to first value
 * \param p2        Pointer to second value
 * \retval 0        First value is equal to second value
 * \retval -1       First value is less than second value
 * \retval 1        First value is greater than second value
 */
static int gdt_compare_string(const void * p1, const void * p2);

/*!
 * \brief           Compare function for GDSString
 * \param p1        Pointer to first value
 * \param p2        Pointer to second value
 * \retval 0        First value is equal to second value
 * \retval -1       First value is less than second value
 * \retval 1        First value is greater than second value
 */
static int gdt_compare_gds_str(const void * p1, const void * p2);

void gdt_set_value(struct gdt_generic_datatype * data,
                   const enum gds_datatype type, gds_cfunc cfunc, va_list ap)
{
    data->type = type;

    switch ( type ) {
        case DATATYPE_CHAR:
            data->compfunc = gdt_compare_char;
            data->data.c = (char) va_arg(ap, int);
            break;

        case DATATYPE_SIGNED_CHAR:
            data->compfunc = gdt_compare_schar;
            data->data.sc = (signed char) va_arg(ap, int);
            break;

        case DATATYPE_UNSIGNED_CHAR:
            data->compfunc = gdt_compare_uchar;
            data->data.uc = (unsigned char) va_arg(ap, int);
            break;

        case DATATYPE_INT:
            data->compfunc = gdt_compare_int;
            data->data.i = va_arg(ap, int);
            break;

        case DATATYPE_UNSIGNED_INT:
            data->compfunc = gdt_compare_uint;
            data->data.ui = va_arg(ap, unsigned int);
            break;

        case DATATYPE_LONG:
            data->compfunc = gdt_compare_long;
            data->data.l = va_arg(ap, long);
            break;

        case DATATYPE_UNSIGNED_LONG:
            data->compfunc = gdt_compare_ulong;
            data->data.ul = va_arg(ap, unsigned long);
            break;

        case DATATYPE_LONG_LONG:
            data->compfunc = gdt_compare_longlong;
            data->data.ll = va_arg(ap, long long);
            break;

        case DATATYPE_UNSIGNED_LONG_LONG:
            data->compfunc = gdt_compare_ulonglong;
            data->data.ull = va_arg(ap, unsigned long long);
            break;

        case DATATYPE_SIZE_T:
            data->compfunc = gdt_compare_sizet;
            data->data.st = va_arg(ap, size_t);
            break;

        case DATATYPE_DOUBLE:
            data->compfunc = gdt_compare_double;
            data->data.d = va_arg(ap, double);
            break;

        case DATATYPE_STRING:
            data->compfunc = gdt_compare_string;
            data->data.pc = va_arg(ap, char *);
            break;

        case DATATYPE_GDSSTRING:
            data->compfunc = gdt_compare_gds_str;
            data->data.gdsstr = va_arg(ap, GDSString);
            break;

        case DATATYPE_POINTER:
            data->compfunc = cfunc;
            data->data.p = va_arg(ap, void *);
            break;

        default:
            abort_error("gds library", "unrecognized datatype");
            break;
    }
}

void gdt_get_value(const struct gdt_generic_datatype * data, void * p)
{
    switch ( data->type ) {
        case DATATYPE_CHAR:
            *((char *) p) = data->data.c;
            break;

        case DATATYPE_SIGNED_CHAR:
            *((signed char *) p) = data->data.sc;
            break;

        case DATATYPE_UNSIGNED_CHAR:
            *((unsigned char *) p) = data->data.uc;
            break;

        case DATATYPE_INT:
            *((int *) p) = data->data.i;
            break;

        case DATATYPE_UNSIGNED_INT:
            *((unsigned int *) p) = data->data.ui;
            break;

        case DATATYPE_LONG:
            *((long *) p) = data->data.l;
            break;

        case DATATYPE_UNSIGNED_LONG:
            *((unsigned long *) p) = data->data.ul;
            break;

        case DATATYPE_LONG_LONG:
            *((long long *) p) = data->data.ll;
            break;

        case DATATYPE_UNSIGNED_LONG_LONG:
            *((unsigned long long *) p) = data->data.ull;
            break;

        case DATATYPE_SIZE_T:
            *((size_t *) p) = data->data.st;
            break;

        case DATATYPE_DOUBLE:
            *((double *) p) = data->data.d;
            break;

        case DATATYPE_STRING:
            *((char **) p) = data->data.pc;
            break;

        case DATATYPE_GDSSTRING:
            *((GDSString *) p) = data->data.gdsstr;
            break;

        case DATATYPE_POINTER:
            *((void **) p) = data->data.p;
            break;

        default:
            abort_error("gds library", "unrecognized datatype");
            break;
    }
}

void gdt_free(struct gdt_generic_datatype * data)
{
    /*  There's no functional reason to NULL the pointers after
     *  freeing them, but it may help cause a segfault if a
     *  later attempt is made to access them, so go ahead and
     *  NULL them for debugging purposes.                        */

    if ( data->type == DATATYPE_POINTER ) {
        free(data->data.p);
        data->data.p = NULL;
    }
    else if ( data->type == DATATYPE_GDSSTRING ) {
        gds_str_destroy(data->data.gdsstr);
        data->data.gdsstr = NULL;
    }
    else if ( data->type == DATATYPE_STRING ) {
        free(data->data.pc);
        data->data.pc = NULL;
    }
}

int gdt_compare(const struct gdt_generic_datatype * d1,
                const struct gdt_generic_datatype * d2)
{
    if ( d1->type != d2->type ) {

        /*  Disallow comparing different types, for simplicity,
         *  and because each data structure contains a single type.  */

        abort_error("gds library", "types are not compatible");
    }

    gds_cfunc cfunc = d1->compfunc;

    if ( d1->type == DATATYPE_CHAR ) {
        return cfunc(&d1->data.c, &d2->data.c);
    }
    else if ( d1->type == DATATYPE_UNSIGNED_CHAR ) {
        return cfunc(&d1->data.uc, &d2->data.uc);
    }
    else if ( d1->type == DATATYPE_SIGNED_CHAR ) {
        return cfunc(&d1->data.sc, &d2->data.sc);
    }
    else if ( d1->type == DATATYPE_INT ) {
        return cfunc(&d1->data.i, &d2->data.i);
    }
    else if ( d1->type == DATATYPE_UNSIGNED_INT ) {
        return cfunc(&d1->data.ui, &d2->data.ui);
    }
    else if ( d1->type == DATATYPE_LONG ) {
        return cfunc(&d1->data.l, &d2->data.l);
    }
    else if ( d1->type == DATATYPE_UNSIGNED_LONG ) {
        return cfunc(&d1->data.ul, &d2->data.ul);
    }
    else if ( d1->type == DATATYPE_LONG_LONG ) {
        return cfunc(&d1->data.ll, &d2->data.ll);
    }
    else if ( d1->type == DATATYPE_UNSIGNED_LONG_LONG ) {
        return cfunc(&d1->data.ull, &d2->data.ull);
    }
    else if ( d1->type == DATATYPE_SIZE_T ) {
        return cfunc(&d1->data.st, &d2->data.st);
    }
    else if ( d1->type == DATATYPE_DOUBLE ) {
        return cfunc(&d1->data.d, &d2->data.d);
    }
    else if ( d1->type == DATATYPE_STRING ) {
        return cfunc(&d1->data.pc, &d2->data.pc);
    }
    else if ( d1->type == DATATYPE_GDSSTRING ) {
        return cfunc(&d1->data.gdsstr, &d2->data.gdsstr);
    }
    else if ( d1->type == DATATYPE_POINTER ) {
        return cfunc(&d1->data.p, &d2->data.p);
    }
    else {
        abort_error("gds library", "unrecognized type");
    }

    return 0;
}

int gdt_compare_void(const void * p1, const void * p2)
{
    return gdt_compare(p1, p2);
}

int gdt_reverse_compare_void(const void * p1, const void * p2)
{
    /*  Just switch the order of the pointers  */

    return gdt_compare(p2, p1);
}

static int gdt_compare_char(const void * p1, const void * p2)
{
    const char c1 = *((const char *) p1);
    const char c2 = *((const char *) p2);
    
    if ( c1 < c2 ) {
        return -1;
    }
    else if ( c1 > c2 ) {
        return 1;
    }
    else {
        return 0;
    }
}

static int gdt_compare_uchar(const void * p1, const void * p2)
{
    const unsigned char c1 = *((const unsigned char *) p1);
    const unsigned char c2 = *((const unsigned char *) p2);
    
    if ( c1 < c2 ) {
        return -1;
    }
    else if ( c1 > c2 ) {
        return 1;
    }
    else {
        return 0;
    }
}

static int gdt_compare_schar(const void * p1, const void * p2)
{
    const signed char c1 = *((const signed char *) p1);
    const signed char c2 = *((const signed char *) p2);
    
    if ( c1 < c2 ) {
        return -1;
    }
    else if ( c1 > c2 ) {
        return 1;
    }
    else {
        return 0;
    }
}

static int gdt_compare_int(const void * p1, const void * p2)
{
    const int n1 = *((const int *) p1);
    const int n2 = *((const int *) p2);
    
    if ( n1 < n2 ) {
        return -1;
    }
    else if ( n1 > n2 ) {
        return 1;
    }
    else {
        return 0;
    }
}

static int gdt_compare_uint(const void * p1, const void * p2)
{
    const unsigned int n1 = *((const unsigned int *) p1);
    const unsigned int n2 = *((const unsigned int *) p2);
    
    if ( n1 < n2 ) {
        return -1;
    }
    else if ( n1 > n2 ) {
        return 1;
    }
    else {
        return 0;
    }
}

static int gdt_compare_long(const void * p1, const void * p2)
{
    const long n1 = *((const long *) p1);
    const long n2 = *((const long *) p2);
    
    if ( n1 < n2 ) {
        return -1;
    }
    else if ( n1 > n2 ) {
        return 1;
    }
    else {
        return 0;
    }
}

static int gdt_compare_ulong(const void * p1, const void * p2)
{
    const unsigned long n1 = *((const unsigned long *) p1);
    const unsigned long n2 = *((const unsigned long *) p2);
    
    if ( n1 < n2 ) {
        return -1;
    }
    else if ( n1 > n2 ) {
        return 1;
    }
    else {
        return 0;
    }
}

static int gdt_compare_longlong(const void * p1, const void * p2)
{
    const long long n1 = *((const long long *) p1);
    const long long n2 = *((const long long *) p2);
    
    if ( n1 < n2 ) {
        return -1;
    }
    else if ( n1 > n2 ) {
        return 1;
    }
    else {
        return 0;
    }
}

static int gdt_compare_ulonglong(const void * p1, const void * p2)
{
    const unsigned long long n1 = *((const unsigned long long *) p1);
    const unsigned long long n2 = *((const unsigned long long *) p2);
    
    if ( n1 < n2 ) {
        return -1;
    }
    else if ( n1 > n2 ) {
        return 1;
    }
    else {
        return 0;
    }
}

static int gdt_compare_sizet(const void * p1, const void * p2)
{
    const size_t s1 = *((const size_t *) p1);
    const size_t s2 = *((const size_t *) p2);
    
    if ( s1 < s2 ) {
        return -1;
    }
    else if ( s1 > s2 ) {
        return 1;
    }
    else {
        return 0;
    }
}

static int gdt_compare_double(const void * p1, const void * p2)
{
    const double d1 = *((const double *) p1);
    const double d2 = *((const double *) p2);
    
    if ( d1 < d2 ) {
        return -1;
    }
    else if ( d1 > d2 ) {
        return 1;
    }
    else {
        return 0;
    }
}

static int gdt_compare_string(const void * p1, const void * p2)
{
    const char * s1 = *((const char **) p1);
    const char * s2 = *((const char **) p2);

    return strcmp(s1, s2);
}

static int gdt_compare_gds_str(const void * p1, const void * p2)
{
    GDSString s1 = *((GDSString *) p1);
    GDSString s2 = *((GDSString *) p2);

    return gds_str_compare(s1, s2);
}
