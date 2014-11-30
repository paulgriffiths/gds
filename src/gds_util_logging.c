/*!
 * \file            gds_util_logging.c
 * \brief           Implementation of logging functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <assert.h>
#include <pggds/gds_util.h>

static FILE * gds_error_file = NULL;

FILE * gds_errlog(void)
{
    return gds_error_file ? gds_error_file : stderr;
}
