/*!
 * \file            test_logging.h
 * \brief           Interface to unit test logging functionality.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_TEST_LOGGING_H
#define PG_TEST_LOGGING_H

#include <string.h>
#include <stdbool.h>

/*!
 * \brief           Macro for defining a test suite.
 * \details         This macro should be called prior to defining any
 * test cases.
 * \param name      The name of the test suite.
 */
#define TEST_SUITE(name) static const char * const \
    izzywig_testsuitename = (#name)

/*!
 * \brief           Macro for defining a test case.
 * \param name      The name of the test case.
 */
#define TEST_CASE(name) static const char * const \
    name##_testcasename = (#name); \
    static void name(const char * const izzywig_testcasename)

/*!
 * \brief           Macro to run a test case.
 * \param name      The name of the test case, as previously defined by
 * a call to TEST_CASE().
 */
#define RUN_CASE(name) name(name##_testcasename)

/*!
 * \brief           Macro to test if a given condition is true.
 * \param cond      The condition to test.
 */
#define TEST_ASSERT_TRUE(cond) tests_assert_true((cond), \
        izzywig_testsuitename, \
        izzywig_testcasename, \
        (#cond " is not true"), \
        __FILE__, \
        __LINE__)

/*!
 * \brief           Macro to test if a given condition is false.
 * \param cond      The condition to test.
 */
#define TEST_ASSERT_FALSE(cond) tests_assert_true(!(cond), \
        izzywig_testsuitename, \
        izzywig_testcasename, \
        (#cond " is not false"), \
        __FILE__, \
        __LINE__)

/*!
 * \brief           Macro to test if two values are equal.
 * \param a         The first value.
 * \param b         The second value.
 */
#define TEST_ASSERT_EQUAL(a,b) tests_assert_true(((a)==(b)), \
        izzywig_testsuitename, \
        izzywig_testcasename, \
        (#a " is not equal to " #b), \
        __FILE__, \
        __LINE__)

/*!
 * \brief           Macro to test if two values are not equal.
 * \param a         The first value.
 * \param b         The second value.
 */
#define TEST_ASSERT_NOTEQUAL(a,b) tests_assert_true(((a)!=(b)), \
        izzywig_testsuitename, \
        izzywig_testcasename, \
        (#a " is equal to " #b), \
        __FILE__, \
        __LINE__)

/*!
 * \brief           Macro to test if two strings are equal.
 * \param s1        The first string.
 * \param s2        The second string.
 */
#define TEST_ASSERT_STR_EQUAL(s1,s2) tests_assert_true(!strcmp((s1),(s2)), \
        izzywig_testsuitename, \
        izzywig_testcasename, \
        (#s1 " is not equal to " #s2), \
        __FILE__, \
        __LINE__)

/*!
 * \brief           Macro to test if two strings are not equal.
 * \param s1        The first string.
 * \param s2        The second string.
 */
#define TEST_ASSERT_STR_NOTEQUAL(s1,s2) tests_assert_true(strcmp((s1),(s2)), \
        izzywig_testsuitename, \
        izzywig_testcasename, \
        (#s1 " is equal to " #s2), \
        __FILE__, \
        __LINE__)

/*!
 * \brief               Logs the result of a true/false unit test.
 * \details             A message is output to standard error on test
 * failure, showing the suite and case name, the source file and line
 * of the test, and a message. This function is designed to be called
 * via one of the TEST_ macros, and in most cases should not be called
 * directly.
 * \param success       The test condition.
 * \param suitname      The name of the test suite.
 * \param casename      The name of the test case.
 * \param failmessage   The message to print on test failure.
 * \param filename      The name of the file containing the test.
 * \param linenum       The source file line number containing the test.
 */
void tests_assert_true(const bool success, const char * suitename,
                       const char * casename, const char * failmessage,
                       const char * filename, const int linenum);

/*!
 * \brief           Logs the result of a unit test.
 * \param success   `true` if the test succeeded, `false` otherwise.
 * \param fmt       Format string for failure message.
 * \param ...       Arguements to format string.
 */
void tests_log_test(const bool success, const char * fmt, ...);

/*!
 * \brief           Initializes the test runner.
 */

void tests_initialize(void);

/*!
 * \brief           Reports on the test results.
 */

void tests_report(void);

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
