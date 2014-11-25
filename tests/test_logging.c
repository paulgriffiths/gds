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

/*!
 * \brief           Logs the result of a single test.
 * \param success   `true` if the test passed, `false` if it failed.
 */
static void tests_log_single_test(const bool success)
{
    ++total_tests;
    if ( success ) {
        ++test_successes;
    }
    else {
        ++test_failures;
    }
}

void tests_assert_true(const bool success, const char * suitename,
                       const char * casename, const char * failmessage,
                       const char * filename, const int linenum)
{
    tests_log_single_test(success);

    if ( show_failures && !success ) {
        fprintf(stderr, "Test failed at %s line %d\n", filename, linenum);
        fprintf(stderr, "  Suite '%s', case '%s'\n", suitename, casename);
        fprintf(stderr, "  %s\n\n", failmessage);
    }
}

void tests_log_test(const bool success, const char * fmt, ...) {
    tests_log_single_test(success);

    if ( show_failures && !success ) {
        fprintf(stderr, "Failure (%d): ", total_tests);
        va_list ap;
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        va_end(ap);
        fprintf(stderr, "\n");
    }
}

void tests_initialize(void)
{
    total_tests = 0;
    test_successes = 0;
    test_failures = 0;
}

void tests_report(void)
{
    fprintf(stderr, "%d successes and %d failures from %d tests.\n",
            test_successes, test_failures, total_tests);
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
