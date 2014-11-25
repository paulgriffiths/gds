#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_util.h"
#include "test_string_util.h"
#include "test_logging.h"

/*  Helper functions  */

static struct pair_string * xpair_string_create(const char * str, const char d)
{
    struct pair_string * ps = pair_string_create(str, d);
    if ( !ps ) {
        perror("couldn't allocate memory for pair string");
        exit(EXIT_FAILURE);
    }
    return ps;
}

static struct pair_string * xpair_string_copy(struct pair_string * pair)
{
    struct pair_string * ps = pair_string_copy(pair);
    if ( !ps ) {
        perror("couldn't allocate memory for pair string");
        exit(EXIT_FAILURE);
    }
    return ps;
}

static struct list_string * xsplit_string(const char * str, const char d)
{
    struct list_string * list = split_string(str, d);
    if ( !list ) {
        perror("couldn't allocate memory for string list");
        exit(EXIT_FAILURE);
    }
    return list;
}

/*  Tests  */

TEST_SUITE(test_string_util);

TEST_CASE(test_trim_left)
{
    char control[20];
    
    strcpy(control, "");
    TEST_ASSERT_STR_EQUAL(gds_trim_left(control), "");

    strcpy(control, "   ");
    TEST_ASSERT_STR_EQUAL(gds_trim_left(control), "");

    strcpy(control, "two words");
    TEST_ASSERT_STR_EQUAL(gds_trim_left(control), "two words");

    strcpy(control, "   two words");
    TEST_ASSERT_STR_EQUAL(gds_trim_left(control), "two words");

    strcpy(control, "two words  ");
    TEST_ASSERT_STR_EQUAL(gds_trim_left(control), "two words  ");

    strcpy(control, "   two words  ");
    TEST_ASSERT_STR_EQUAL(gds_trim_left(control), "two words  ");
    
    strcpy(control, "\t   two words");
    TEST_ASSERT_STR_EQUAL(gds_trim_left(control), "two words");
    
    strcpy(control, "\n   two words");
    TEST_ASSERT_STR_EQUAL(gds_trim_left(control), "two words");
    
    strcpy(control, "\r   two words");
    TEST_ASSERT_STR_EQUAL(gds_trim_left(control), "two words");
}

TEST_CASE(test_trim_right)
{
    char control[20];
    
    strcpy(control, "");
    TEST_ASSERT_STR_EQUAL(gds_trim_right(control), "");

    strcpy(control, "   ");
    TEST_ASSERT_STR_EQUAL(gds_trim_right(control), "");

    strcpy(control, "two words");
    TEST_ASSERT_STR_EQUAL(gds_trim_right(control), "two words");

    strcpy(control, "   two words");
    TEST_ASSERT_STR_EQUAL(gds_trim_right(control), "   two words");

    strcpy(control, "two words  ");
    TEST_ASSERT_STR_EQUAL(gds_trim_right(control), "two words");

    strcpy(control, "   two words  ");
    TEST_ASSERT_STR_EQUAL(gds_trim_right(control), "   two words");
    
    strcpy(control, "two words  \t");
    TEST_ASSERT_STR_EQUAL(gds_trim_right(control), "two words");
    
    strcpy(control, "two words  \n");
    TEST_ASSERT_STR_EQUAL(gds_trim_right(control), "two words");
    
    strcpy(control, "two words  \r");
    TEST_ASSERT_STR_EQUAL(gds_trim_right(control), "two words");
}

TEST_CASE(test_trim)
{
    char control[20];
    
    strcpy(control, "");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "");

    strcpy(control, "   ");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "");

    strcpy(control, "two words");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "two words");

    strcpy(control, "   two words");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "two words");

    strcpy(control, "two words  ");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "two words");

    strcpy(control, "   two words  ");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "two words");
    
    strcpy(control, "two words  \t");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "two words");
    
    strcpy(control, "two words  \n");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "two words");
    
    strcpy(control, "two words  \r");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "two words");

    strcpy(control, "\t   two words");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "two words");
    
    strcpy(control, "\n   two words");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "two words");
    
    strcpy(control, "\r   two words");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "two words");

    strcpy(control, "\t   two words  \n");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "two words");
    
    strcpy(control, "\n   two words  \r");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "two words");
    
    strcpy(control, "\r   two words  \n");
    TEST_ASSERT_STR_EQUAL(gds_trim(control), "two words");
}

TEST_CASE(test_pair_string)
{
    /*  Test basic functioning  */

    struct pair_string * ps = xpair_string_create("key=value", '=');
    TEST_ASSERT_STR_EQUAL(ps->first, "key");
    TEST_ASSERT_STR_EQUAL(ps->second, "value");
    pair_string_destroy(ps);

    /*  Test for leading or trailing whitespace  */

    ps = xpair_string_create("  chalk/cheese  ", '/');
    TEST_ASSERT_STR_EQUAL(ps->first, "  chalk");
    TEST_ASSERT_STR_EQUAL(ps->second, "cheese  ");
    pair_string_destroy(ps);

    /*  Test for leading and trailing whitespace  */

    ps = xpair_string_create(" apples   &   pairs ", '&');
    TEST_ASSERT_STR_EQUAL(ps->first, " apples   ");
    TEST_ASSERT_STR_EQUAL(ps->second, "   pairs ");
    pair_string_destroy(ps);

    /*  Test for repeated delimiter character  */

    ps = xpair_string_create("one=two=three", '=');
    TEST_ASSERT_STR_EQUAL(ps->first, "one");
    TEST_ASSERT_STR_EQUAL(ps->second, "two=three");
    pair_string_destroy(ps);

    /*  Test for only whitespace in value  */

    ps = xpair_string_create(" spaces :   ", ':');
    TEST_ASSERT_STR_EQUAL(ps->first, " spaces ");
    TEST_ASSERT_STR_EQUAL(ps->second, "   ");
    pair_string_destroy(ps);

    /*  Test for empty value with delimiter character  */

    ps = xpair_string_create("empty-", '-');
    TEST_ASSERT_STR_EQUAL(ps->first, "empty");
    TEST_ASSERT_STR_EQUAL(ps->second, "");
    pair_string_destroy(ps);

    /*  Test for no delimiter character  */

    ps = xpair_string_create("reallyempty", '=');
    TEST_ASSERT_STR_EQUAL(ps->first, "reallyempty");
    TEST_ASSERT_STR_EQUAL(ps->second, "");
    pair_string_destroy(ps);

    /*  Test for empty string  */

    ps = xpair_string_create("", '=');
    TEST_ASSERT_STR_EQUAL(ps->first, "");
    TEST_ASSERT_STR_EQUAL(ps->second, "");
    pair_string_destroy(ps);

    /*  Test for only the delimiter character  */

    ps = xpair_string_create("=", '=');
    TEST_ASSERT_STR_EQUAL(ps->first, "");
    TEST_ASSERT_STR_EQUAL(ps->second, "");
    pair_string_destroy(ps);
}

TEST_CASE(test_pair_string_copy)
{
    /*  Test basic functioning  */

    struct pair_string * ps = xpair_string_create("key=value", '=');
    struct pair_string * psc = xpair_string_copy(ps);
    pair_string_destroy(ps);

    TEST_ASSERT_STR_EQUAL(psc->first, "key");
    TEST_ASSERT_STR_EQUAL(psc->second, "value");
    pair_string_destroy(psc);

    /*  Test with empty value  */

    ps = xpair_string_create("key=", '=');
    psc = xpair_string_copy(ps);
    pair_string_destroy(ps);

    TEST_ASSERT_STR_EQUAL(psc->first, "key");
    TEST_ASSERT_STR_EQUAL(psc->second, "");
    pair_string_destroy(psc);

    /*  Test with empty string  */

    ps = xpair_string_create("", '=');
    psc = xpair_string_copy(ps);
    pair_string_destroy(ps);

    TEST_ASSERT_STR_EQUAL(psc->first, "");
    TEST_ASSERT_STR_EQUAL(psc->second, "");
    pair_string_destroy(psc);
}

TEST_CASE(test_split_string)
{
    /*  Test basic functionality  */

    struct list_string * list = xsplit_string("one,two,three,four", ',');
    TEST_ASSERT_EQUAL(list->size, 4);
    TEST_ASSERT_STR_EQUAL(list->list[0], "one");
    TEST_ASSERT_STR_EQUAL(list->list[1], "two");
    TEST_ASSERT_STR_EQUAL(list->list[2], "three");
    TEST_ASSERT_STR_EQUAL(list->list[3], "four");
    TEST_ASSERT_EQUAL(list->list[4], NULL);
    list_string_destroy(list);

    /*  Test with empty string  */

    list = xsplit_string("", ':');
    TEST_ASSERT_EQUAL(list->size, 0);
    TEST_ASSERT_EQUAL(list->list[0], NULL);
    list_string_destroy(list);

    /*  Test without delimiter character  */

    list = xsplit_string("single", '/');
    TEST_ASSERT_EQUAL(list->size, 1);
    TEST_ASSERT_STR_EQUAL(list->list[0], "single");
    TEST_ASSERT_EQUAL(list->list[1], NULL);
    list_string_destroy(list);

    /*  Test with one trailing delimiter character  */

    list = xsplit_string("singlewithdelim&", '&');
    TEST_ASSERT_EQUAL(list->size, 1);
    TEST_ASSERT_STR_EQUAL(list->list[0], "singlewithdelim");
    TEST_ASSERT_EQUAL(list->list[1], NULL);
    list_string_destroy(list);

    /*  Test with several delimiter characters, one trailing  */

    list = xsplit_string("three&with&delim&", '&');
    TEST_ASSERT_EQUAL(list->size, 3);
    TEST_ASSERT_STR_EQUAL(list->list[0], "three");
    TEST_ASSERT_STR_EQUAL(list->list[1], "with");
    TEST_ASSERT_STR_EQUAL(list->list[2], "delim");
    TEST_ASSERT_EQUAL(list->list[3], NULL);
    list_string_destroy(list);
    
    /*  Test with internal empty field  */

    list = xsplit_string("missing&&field", '&');
    TEST_ASSERT_EQUAL(list->size, 3);
    TEST_ASSERT_STR_EQUAL(list->list[0], "missing");
    TEST_ASSERT_STR_EQUAL(list->list[1], "");
    TEST_ASSERT_STR_EQUAL(list->list[2], "field");
    TEST_ASSERT_EQUAL(list->list[3], NULL);
    list_string_destroy(list);

    /*  Test with internal empty fields  */

    list = xsplit_string("missing&&&fields", '&');
    TEST_ASSERT_EQUAL(list->size, 4);
    TEST_ASSERT_STR_EQUAL(list->list[0], "missing");
    TEST_ASSERT_STR_EQUAL(list->list[1], "");
    TEST_ASSERT_STR_EQUAL(list->list[2], "");
    TEST_ASSERT_STR_EQUAL(list->list[3], "fields");
    TEST_ASSERT_EQUAL(list->list[4], NULL);
    list_string_destroy(list);
}

void test_string_util(void)
{
    RUN_CASE(test_trim_left);
    RUN_CASE(test_trim_right);
    RUN_CASE(test_trim);
    RUN_CASE(test_pair_string);
    RUN_CASE(test_pair_string_copy);
    RUN_CASE(test_split_string);
}
