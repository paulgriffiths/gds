/*  Unit tests for command line options  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pggds/gds_opt.h>
#include <pggds/unittest.h>
#include "test_options.h"

TEST_SUITE(test_options);

/*  Test parsing of allowed options  */

TEST_CASE(test_options_allowed)
{
    char * argv[] = {"dummy", NULL};

    /*  Test with empty string  */

    TEST_ASSERT_TRUE(gds_parse_options("", argv));
    gds_free_options();

    /*  Test with one option, no arguments  */

    TEST_ASSERT_TRUE(gds_parse_options("a", argv));
    gds_free_options();

    /*  Test with multiple options, no arguments  */

    TEST_ASSERT_TRUE(gds_parse_options("ab", argv));
    gds_free_options();

    /*  Test with one option, one argument  */

    TEST_ASSERT_TRUE(gds_parse_options("a:", argv));
    gds_free_options();

    /*  Test with multiple options and argument(s)  */

    TEST_ASSERT_TRUE(gds_parse_options("a:b", argv));
    gds_free_options();

    TEST_ASSERT_TRUE(gds_parse_options("ab:", argv));
    gds_free_options();

    TEST_ASSERT_TRUE(gds_parse_options("ab:c", argv));
    gds_free_options();

    TEST_ASSERT_TRUE(gds_parse_options("a:b:c", argv));
    gds_free_options();

    /*  Test failure on bad placement of argument character  */

    TEST_ASSERT_FALSE(gds_parse_options(":", argv));
    TEST_ASSERT_FALSE(gds_parse_options(":a", argv));
    TEST_ASSERT_FALSE(gds_parse_options("a::", argv));
    TEST_ASSERT_FALSE(gds_parse_options("a::b", argv));

    /*  Test failure on illegal characters  */

    TEST_ASSERT_FALSE(gds_parse_options("ab,", argv));
    TEST_ASSERT_FALSE(gds_parse_options(";ab", argv));
    TEST_ASSERT_FALSE(gds_parse_options("a!b", argv));
    TEST_ASSERT_FALSE(gds_parse_options("ab3", argv));
}

TEST_CASE(test_options_present)
{
    char * argv[] = {"dummy", "-a", "-b42", NULL};

    /*  Test with empty string  */

    TEST_ASSERT_TRUE(gds_parse_options("ab:h", argv));
    TEST_ASSERT_TRUE(gds_option_present("a"));
    TEST_ASSERT_TRUE(gds_option_present("b"));
    TEST_ASSERT_FALSE(gds_option_present("h"));
    TEST_ASSERT_FALSE(gds_option_present("q"));

    gds_free_options();
}

TEST_CASE(test_options_argument_string)
{
    char * argv[] = {"dummy", "-a", "-b42", "-c", NULL};

    /*  Test with empty string  */

    TEST_ASSERT_TRUE(gds_parse_options("ab:c:h", argv));
    TEST_ASSERT_FALSE(gds_option_argument_string("a"));
    TEST_ASSERT_EQUAL(gds_option_argument_string("a"), NULL);
    TEST_ASSERT_TRUE(gds_option_argument_string("b"));
    TEST_ASSERT_STR_EQUAL(gds_option_argument_string("b"), "42");
    TEST_ASSERT_TRUE(gds_option_argument_string("c"));
    TEST_ASSERT_STR_EQUAL(gds_option_argument_string("c"), "");

    gds_free_options();
}

TEST_CASE(test_options_argument_int)
{
    char * argv[] = {"dummy", "-a", "-b42", "-chaha", NULL};

    /*  Test with empty string  */

    TEST_ASSERT_TRUE(gds_parse_options("ab:c:h", argv));
    TEST_ASSERT_FALSE(gds_option_argument_string("a"));
    TEST_ASSERT_EQUAL(gds_option_argument_string("a"), NULL);
    TEST_ASSERT_TRUE(gds_option_argument_string("b"));
    TEST_ASSERT_STR_EQUAL(gds_option_argument_string("b"), "42");
    TEST_ASSERT_TRUE(gds_option_argument_string("c"));
    TEST_ASSERT_STR_EQUAL(gds_option_argument_string("c"), "haha");

    int n;
    TEST_ASSERT_TRUE(gds_option_argument_int("b", &n));
    TEST_ASSERT_EQUAL(n, 42);
    TEST_ASSERT_FALSE(gds_option_argument_int("c", NULL));

    gds_free_options();
}

TEST_CASE(test_options_progname)
{
    char * argv[] = {"dummy", "-a", "-b42", "-chaha", NULL};

    TEST_ASSERT_TRUE(gds_parse_options("ab:c:h", argv));
    TEST_ASSERT_STR_EQUAL(gds_option_progname(), "dummy");

    gds_free_options();
}

TEST_CASE(test_options_nonopts_number)
{
    char * argv[] = {"dummy", "-a", "-b42", "-chaha", NULL};

    TEST_ASSERT_TRUE(gds_parse_options("ab:c:h", argv));
    TEST_ASSERT_EQUAL(gds_option_nonopts_number(), 0);

    gds_free_options();

    char * argv2[] = {"dummy", "-a", "non1", "-b42", "-chaha", "non2", NULL};

    TEST_ASSERT_TRUE(gds_parse_options("ab:c:h", argv2));
    TEST_ASSERT_EQUAL(gds_option_nonopts_number(), 2);

    gds_free_options();
}

TEST_CASE(test_options_nonopts)
{
    char * argv[] = {"dummy", "-a", "non1", "-b42", "-chaha", "non2", NULL};

    TEST_ASSERT_TRUE(gds_parse_options("ab:c:h", argv));
    TEST_ASSERT_EQUAL(gds_option_nonopts_number(), 2);
    TEST_ASSERT_STR_EQUAL(gds_option_nonopt(0), "non1");
    TEST_ASSERT_STR_EQUAL(gds_option_nonopt(1), "non2");
    TEST_ASSERT_FALSE(gds_option_nonopt(2));

    gds_free_options();
}

void test_options(void)
{
    RUN_CASE(test_options_allowed);
    RUN_CASE(test_options_present);
    RUN_CASE(test_options_argument_string);
    RUN_CASE(test_options_argument_int);
    RUN_CASE(test_options_progname);
    RUN_CASE(test_options_nonopts_number);
    RUN_CASE(test_options_nonopts);
}
