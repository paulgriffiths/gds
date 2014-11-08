/*!
 * \file            test_logging.h
 * \brief           Interface to unit test logging functionality.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_TEST_LOGGING_H
#define PG_TEST_LOGGING_H

#include <stdbool.h>

/*!
 * \brief           Logs the result of a unit test.
 * \param success   `true` if the test succeeded, `false` otherwise.
 * \param fmt       Format string for failure message.
 * \param ...       Arguements to format string.
 */
void tests_log_test(const bool success, const char * fmt, ...);

/*!
 * \brief           Returns the total number of tests run.
 * \returns         The total number of tests run.
 */
int tests_get_total_tests(void);

/*!
 * \brief           Returns the total number of successful tests.
 * \returns         The total number of successful tests.
 */
int tests_get_successes(void);

/*!
 * \brief           Returns the total number of failed tests.
 * \returns         The total number of failed tests.
 */
int tests_get_failures(void);

#endif      /*  PG_TEST_LOGGING_H  */

