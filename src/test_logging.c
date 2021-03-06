/*!
 * \file            test_logging.c
 * \brief           Implementation of unit test logging functionality.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdarg.h>
#include <pggds/gds_util.h>
#include <pggds/test_logging.h>

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
        fprintf(gds_errlog(), "Test failed at %s line %d\n", filename, linenum);
        fprintf(gds_errlog(), "  Suite '%s', case '%s'\n", suitename, casename);
        fprintf(gds_errlog(), "  %s\n\n", failmessage);
    }
}

bool tests_assert_almost_equal(const long double a,
                               const long double b,
                               const long double e)
{
    long double denominator;

    if ( a == 0.0L && b == 0.0L ) {
        return true;
    }
    else if ( a == 0 ) {
        denominator = b;
    }
    else {
        denominator = a;
    }

    const long double eps = denominator * e;
    return fabsl(a - b) <= fabsl(eps);
}

void tests_initialize(void)
{
    total_tests = 0;
    test_successes = 0;
    test_failures = 0;
}

void tests_report(void)
{
    fprintf(gds_errlog(), "%d successes and %d failures from %d tests.\n",
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
