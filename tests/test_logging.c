/*!
 * \file            test_logging.c
 * \brief           Implementation of unit test logging functionality.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include "test_logging.h"

/*!  Number of successful tests  */
static int test_successes = 0;

/*!  Number of failed tests  */
static int test_failures = 0;

/*!  Total number of tests  */
static int total_tests = 0;

/*!  Control flag to display individual test failures  */
static bool show_failures = true;

void tests_log_test(const bool success, const char * fmt, ...) {
    ++total_tests;
    if ( success ) {
        ++test_successes;
    }
    else {
        ++test_failures;
    }

    if ( show_failures && !success ) {
        fprintf(stderr, "Failure (%d): ", total_tests);
        va_list ap;
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        va_end(ap);
        fprintf(stderr, "\n");
    }
}

int tests_get_total_tests(void) {
    return total_tests;
}

int tests_get_successes(void) {
    return test_successes;
}

int tests_get_failures(void) {
    return test_failures;
}

