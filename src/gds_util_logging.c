/*!
 * \file            gds_util_logging.c
 * \brief           Implementation of logging functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <pggds/gds_util.h>

/*!  File scope variable to hold current error file pointer  */
static FILE * gds_error_file = NULL;

FILE * gds_errlog(void)
{
    return gds_error_file ? gds_error_file : stderr;
}
