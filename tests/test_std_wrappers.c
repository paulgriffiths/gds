/*  Unit tests for standard function wrappers  */

#include <stdlib.h>
#include <pggds/gds_util.h>
#include <pggds/unittest.h>
#include "test_std_wrappers.h"

TEST_SUITE(test_std_wrappers);

TEST_CASE(test_xmalloc)
{
    char * p = xmalloc(100);
    TEST_ASSERT_TRUE(p);
    memset(p, '?', 100);
    TEST_ASSERT_EQUAL(p[0], '?');
    TEST_ASSERT_EQUAL(p[99], '?');
    free(p);
}

TEST_CASE(test_xcalloc)
{
    char * p = xcalloc(50, 2);
    TEST_ASSERT_TRUE(p);
    TEST_ASSERT_EQUAL(p[0], 0);
    TEST_ASSERT_EQUAL(p[99], 0);
    memset(p, '?', 100);
    TEST_ASSERT_EQUAL(p[0], '?');
    TEST_ASSERT_EQUAL(p[99], '?');
    free(p);
}

TEST_CASE(test_xrealloc)
{
    char * p = xmalloc(100);
    TEST_ASSERT_TRUE(p);
    memset(p, '?', 100);
    TEST_ASSERT_EQUAL(p[0], '?');
    TEST_ASSERT_EQUAL(p[99], '?');

    p = xrealloc(p, 200);
    TEST_ASSERT_TRUE(p);
    memset(p + 100, '*', 100);
    TEST_ASSERT_EQUAL(p[0], '?');
    TEST_ASSERT_EQUAL(p[99], '?');
    TEST_ASSERT_EQUAL(p[100], '*');
    TEST_ASSERT_EQUAL(p[199], '*');

    free(p);
}

TEST_CASE(test_xstrdup)
{
    const char * p = "Hello, world!";
    char * c = xstrdup(p);
    TEST_ASSERT_TRUE(c);
    TEST_ASSERT_STR_EQUAL(c, "Hello, world!");
    free(c);
}

void test_std_wrappers(void)
{
    RUN_CASE(test_xmalloc);
    RUN_CASE(test_xcalloc);
    RUN_CASE(test_xrealloc);
    RUN_CASE(test_xstrdup);
}
