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
 * \ingroup         unittest
 * \details         This macro should be called prior to defining any
 * test cases.
 * \param name      The name of the test suite.
 */
#define TEST_SUITE(name) static const char * const \
    izzywig_testsuitename = (#name)

/*!
 * \brief           Macro for defining a test case.
 * \ingroup         unittest
 * \param name      The name of the test case.
 */
#define TEST_CASE(name) static const char * const \
    name##_testcasename = (#name); \
    static void name(const char * const izzywig_testcasename)

/*!
 * \brief           Macro to run a test case.
 * \ingroup         unittest
 * \param name      The name of the test case, as previously defined by
 * a call to TEST_CASE().
 */
#define RUN_CASE(name) name(name##_testcasename)

/*!
 * \brief           Macro to test if a given condition is true.
 * \ingroup         unittest
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
 * \ingroup         unittest
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
 * \ingroup         unittest
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
 * \ingroup         unittest
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
 * \brief           Macro to test two real numbers for fuzzy equality.
 * \ingroup         unittest
 * \param a         The first number.
 * \param b         The second number.
 * \param e         The equality threshold. The first parameter
 * will be multiplied by this quantity (unless (a) the first parameter is
 * zero, in which case the second parameter will be multiplied by it; or
 * (b) both parameters are zero, in which case the function will return
 * true) and the function will be true if the absolute difference between
 * the first two parameters is smaller than or equal to this value.
 */
#define TEST_ASSERT_ALMOST_EQUAL(a,b,e) tests_assert_true( \
        tests_assert_almost_equal(a, b, e), \
        izzywig_testsuitename, \
        izzywig_testcasename, \
        (#a " is not fuzzily equal to " #b), \
        __FILE__, \
        __LINE__)

/*!
 * \brief           Macro to test if two strings are equal.
 * \ingroup         unittest
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
 * \ingroup         unittest
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
 * \ingroup         unittest
 * \param success       The test condition.
 * \param suitename     The name of the test suite.
 * \param casename      The name of the test case.
 * \param failmessage   The message to print on test failure.
 * \param filename      The name of the file containing the test.
 * \param linenum       The source file line number containing the test.
 */
void tests_assert_true(const bool success, const char * suitename,
                       const char * casename, const char * failmessage,
                       const char * filename, const int linenum);

/*!
 * \brief               Tests two real numbers for fuzzy equality.
 * \ingroup         unittest
 * \param a             The first number.
 * \param b             The second number.
 * \param e             The equality threshold. The first parameter
 * will be multiplied by this quantity (unless (a) the first parameter is
 * zero, in which case the second parameter will be multiplied by it; or
 * (b) both parameters are zero, in which case the function will return
 * true) and the function will be true if the absolute difference between
 * the first two parameters is smaller than or equal to this value.
 * \retval true         The numbers are equal to the specified precision
 * \retval false        The numbers are not equal to the specified precision
 */
bool tests_assert_almost_equal(const long double a,
                               const long double b,
                               const long double e);

/*!
 * \brief           Initializes the test runner.
 * \ingroup         unittest
 */

void tests_initialize(void);

/*!
 * \brief           Reports on the test results.
 * \ingroup         unittest
 */

void tests_report(void);

/*!
 * \brief           Returns the total number of tests run.
 * \ingroup         unittest
 * \returns         The total number of tests run.
 */
int tests_get_total_tests(void);

/*!
 * \brief           Returns the total number of successful tests.
 * \ingroup         unittest
 * \returns         The total number of successful tests.
 */
int tests_get_successes(void);

/*!
 * \brief           Returns the total number of failed tests.
 * \ingroup         unittest
 * \returns         The total number of failed tests.
 */
int tests_get_failures(void);

#endif      /*  PG_TEST_LOGGING_H  */
