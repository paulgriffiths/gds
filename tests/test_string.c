/*  Unit tests for string data structure  */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gds_string.h"
#include "test_string.h"
#include "test_logging.h"

TEST_SUITE(test_string);

/*  Test basic queue operations  */

TEST_CASE(test_string_create_simple)
{
    GDSString str = gds_str_create("my string");

    /*  Test basic attributes  */

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "my string");
    TEST_ASSERT_EQUAL(gds_str_length(str), 9);
    TEST_ASSERT_FALSE(gds_str_is_empty(str));

    gds_str_destroy(str);
}

TEST_CASE(test_string_create_empty)
{
    GDSString str = gds_str_create("");

    /*  Test basic attributes  */

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "");
    TEST_ASSERT_EQUAL(gds_str_length(str), 0);
    TEST_ASSERT_TRUE(gds_str_is_empty(str));

    gds_str_destroy(str);
}

TEST_CASE(test_string_compare)
{
    GDSString strA = gds_str_create("Aardvark");
    GDSString strE = gds_str_create("Elephant");
    GDSString strE2 = gds_str_create("Elephant");
    GDSString strG = gds_str_create("Giraffe");

    /*  Compare for equality, including to themselves  */

    TEST_ASSERT_EQUAL(gds_str_compare(strA, strA), 0);
    TEST_ASSERT_EQUAL(gds_str_compare(strE, strE), 0);
    TEST_ASSERT_EQUAL(gds_str_compare(strE, strE2), 0);
    TEST_ASSERT_EQUAL(gds_str_compare(strG, strG), 0);

    /*  Compare for inequality  */

    TEST_ASSERT_EQUAL(gds_str_compare(strA, strE), -1);
    TEST_ASSERT_EQUAL(gds_str_compare(strE, strA), 1);
    TEST_ASSERT_EQUAL(gds_str_compare(strA, strG), -1);
    TEST_ASSERT_EQUAL(gds_str_compare(strG, strA), 1);
    TEST_ASSERT_EQUAL(gds_str_compare(strE, strG), -1);
    TEST_ASSERT_EQUAL(gds_str_compare(strG, strE), 1);

    gds_str_destroy(strA);
    gds_str_destroy(strE);
    gds_str_destroy(strE2);
    gds_str_destroy(strG);
}

TEST_CASE(test_string_compare_cstring)
{
    GDSString strA = gds_str_create("Aardvark");
    GDSString strE = gds_str_create("Elephant");
    GDSString strG = gds_str_create("Giraffe");

    /*  Compare strings  */

    TEST_ASSERT_EQUAL(gds_str_compare_cstr(strA, "Aardvark"), 0);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(strA, "Echidna"), -1);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(strA, "Goat"), -1);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(strE, "Antelope"), 1);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(strE, "Elephant"), 0);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(strE, "Goat"), -1);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(strG, "Antelope"), 1);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(strG, "Echidna"), 1);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(strG, "Giraffe"), 0);

    gds_str_destroy(strA);
    gds_str_destroy(strE);
    gds_str_destroy(strG);
}

TEST_CASE(test_string_dup)
{
    GDSString str1 = gds_str_create("wysiwig");
    GDSString str2 = gds_str_dup(str1);

    /*  Check internal pointers are different  */

    TEST_ASSERT_NOTEQUAL(gds_str_cstr(str1), gds_str_cstr(str2));

    /*  Check basic properties  */

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str2), "wysiwig");
    TEST_ASSERT_EQUAL(gds_str_compare(str1, str2), 0);
    TEST_ASSERT_EQUAL(gds_str_length(str1), 7);
    TEST_ASSERT_EQUAL(gds_str_length(str2), 7);

    gds_str_destroy(str1);
    gds_str_destroy(str2);
}

TEST_CASE(test_string_create_sprintf)
{
    /*  Test a few different cases  */

    GDSString str = gds_str_create_sprintf("%s", "hello world");
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(str, "hello world"), 0);
    TEST_ASSERT_EQUAL(gds_str_length(str), 11);
    gds_str_destroy(str);

    str = gds_str_create_sprintf("%d %s", 10, "seconds");
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(str, "10 seconds"), 0);
    TEST_ASSERT_EQUAL(gds_str_length(str), 10);
    gds_str_destroy(str);

    str = gds_str_create_sprintf("%.2f %s", 4.5678, "minutes");
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(str, "4.57 minutes"), 0);
    TEST_ASSERT_EQUAL(gds_str_length(str), 12);
    gds_str_destroy(str);

    str = gds_str_create_sprintf("%s %02lu %s", "I have", 3UL, "oranges");
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(str, "I have 03 oranges"), 0);
    TEST_ASSERT_EQUAL(gds_str_length(str), 17);
    gds_str_destroy(str);

    str = gds_str_create_sprintf("It's %02d:%02d:%02d", 2, 12, 3);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(str, "It's 02:12:03"), 0);
    TEST_ASSERT_EQUAL(gds_str_length(str), 13);
    gds_str_destroy(str);
}

TEST_CASE(test_string_assign)
{
    GDSString str1 = gds_str_create("first string");
    GDSString str2 = gds_str_create("second string");

    /*  Check strings are as expected before assignment  */

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str1), "first string");
    TEST_ASSERT_EQUAL(gds_str_length(str1), 12);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str2), "second string");
    TEST_ASSERT_EQUAL(gds_str_length(str2), 13);

    /*  Check strings are as expected after assignment  */

    gds_str_assign(str2, str1);

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str1), "first string");
    TEST_ASSERT_EQUAL(gds_str_length(str1), 12);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str2), "first string");
    TEST_ASSERT_EQUAL(gds_str_length(str2), 12);
    TEST_ASSERT_STR_NOTEQUAL(gds_str_cstr(str2), "second string");
    TEST_ASSERT_NOTEQUAL(gds_str_cstr(str1), gds_str_cstr(str2));

    gds_str_destroy(str1);
    gds_str_destroy(str2);
}

TEST_CASE(test_string_assign_empty)
{
    GDSString str1 = gds_str_create("");
    GDSString str2 = gds_str_create("second string");

    /*  Check strings are as expected before assignment  */

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str1), "");
    TEST_ASSERT_EQUAL(gds_str_length(str1), 0);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str2), "second string");
    TEST_ASSERT_EQUAL(gds_str_length(str2), 13);

    /*  Check strings are as expected after assignment  */

    gds_str_assign(str2, str1);

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str1), "");
    TEST_ASSERT_EQUAL(gds_str_length(str1), 0);
    TEST_ASSERT_TRUE(gds_str_is_empty(str1));
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str2), "");
    TEST_ASSERT_EQUAL(gds_str_length(str2), 0);
    TEST_ASSERT_TRUE(gds_str_is_empty(str2));
    TEST_ASSERT_STR_NOTEQUAL(gds_str_cstr(str2), "second string");
    TEST_ASSERT_NOTEQUAL(gds_str_cstr(str1), gds_str_cstr(str2));

    gds_str_destroy(str1);
    gds_str_destroy(str2);
}

TEST_CASE(test_string_assign_cstr)
{
    GDSString str1 = gds_str_create("first string");
    GDSString str2 = gds_str_create("second string");

    /*  Check strings are as expected before assignment  */

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str1), "first string");
    TEST_ASSERT_EQUAL(gds_str_length(str1), 12);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str2), "second string");
    TEST_ASSERT_EQUAL(gds_str_length(str2), 13);

    /*  Check strings are as expected after assignment  */

    gds_str_assign_cstr(str2, "a third string");

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str1), "first string");
    TEST_ASSERT_EQUAL(gds_str_length(str1), 12);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str2), "a third string");
    TEST_ASSERT_EQUAL(gds_str_length(str2), 14);

    gds_str_destroy(str1);
    gds_str_destroy(str2);
}

TEST_CASE(test_string_assign_cstr_empty)
{
    GDSString str1 = gds_str_create("first string");
    GDSString str2 = gds_str_create("second string");

    /*  Check strings are as expected before assignment  */

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str1), "first string");
    TEST_ASSERT_EQUAL(gds_str_length(str1), 12);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str2), "second string");
    TEST_ASSERT_EQUAL(gds_str_length(str2), 13);

    /*  Check strings are as expected after assignment  */

    gds_str_assign_cstr(str2, "");

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str1), "first string");
    TEST_ASSERT_EQUAL(gds_str_length(str1), 12);
    TEST_ASSERT_FALSE(gds_str_is_empty(str1));
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str2), "");
    TEST_ASSERT_EQUAL(gds_str_length(str2), 0);
    TEST_ASSERT_TRUE(gds_str_is_empty(str2));

    gds_str_destroy(str1);
    gds_str_destroy(str2);
}

TEST_CASE(test_string_concat)
{
    GDSString str1 = gds_str_create("Red ");
    GDSString str2 = gds_str_create("Arrows");

    gds_str_concat(str1, str2);

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str1), "Red Arrows");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str2), "Arrows");

    gds_str_destroy(str1);
    gds_str_destroy(str2);
}

TEST_CASE(test_string_concat_empty)
{
    GDSString str1 = gds_str_create("Nothing else");
    GDSString str2 = gds_str_create("");

    gds_str_concat(str1, str2);

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str1), "Nothing else");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str2), "");

    gds_str_destroy(str1);
    gds_str_destroy(str2);
}

TEST_CASE(test_string_concat_cstr)
{
    GDSString str1 = gds_str_create("Red ");

    gds_str_concat_cstr(str1, "Arrows");

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str1), "Red Arrows");

    gds_str_destroy(str1);
}

TEST_CASE(test_string_concat_cstr_empty)
{
    GDSString str1 = gds_str_create("Nothing else");

    gds_str_concat_cstr(str1, "");

    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str1), "Nothing else");

    gds_str_destroy(str1);
}

TEST_CASE(test_string_trunc)
{
    GDSString str = gds_str_create("These are some words");

    gds_str_trunc(str, 14);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "These are some");
    gds_str_trunc(str, 9);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "These are");
    gds_str_trunc(str, 6);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "These ");
    gds_str_trunc(str, 0);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "");

    gds_str_destroy(str);
}

TEST_CASE(test_string_strchr)
{
    GDSString str = gds_str_create("bongabongajug");

    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'b', 0), 0);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'b', 1), 5);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'b', 2), 5);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'b', 5), 5);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'b', 6), -1);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'o', 0), 1);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'o', 1), 1);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'o', 2), 6);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'o', 6), 6);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'o', 7), -1);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'j', 0), 10);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'u', 0), 11);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'g', 10), 12);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'x', 0), -1);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 'B', 0), -1);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 0, 0), -1);
    TEST_ASSERT_EQUAL(gds_str_strchr(str, 127, 0), -1);

    gds_str_destroy(str);
}

TEST_CASE(test_string_substr_left)
{
    GDSString str = gds_str_create("There once was a frog");

    GDSString sub = gds_str_substr_left(str, 5);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(sub, "There"), 0);
    gds_str_destroy(sub);
    
    sub = gds_str_substr_left(str, 0);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(sub, ""), 0);
    gds_str_destroy(sub);
    
    sub = gds_str_substr_left(str, 17);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(sub, "There once was a "), 0);
    gds_str_destroy(sub);
    
    sub = gds_str_substr_left(str, 21);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(sub, "There once was a frog"), 0);
    gds_str_destroy(sub);

    /*  Test with number of characters greater than length  */
    
    sub = gds_str_substr_left(str, 25);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(sub, "There once was a frog"), 0);
    gds_str_destroy(sub);
    
    /*  Test when original string is empty  */

    gds_str_assign_cstr(str, "");
    sub = gds_str_substr_left(str, 100);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(sub, ""), 0);
    gds_str_destroy(sub);

    gds_str_destroy(str);
}

TEST_CASE(test_string_substr_right)
{
    GDSString str = gds_str_create("Whose name was George");

    GDSString sub = gds_str_substr_right(str, 0);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(sub, ""), 0);
    gds_str_destroy(sub);

    sub = gds_str_substr_right(str, 8);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(sub, "s George"), 0);
    gds_str_destroy(sub);

    sub = gds_str_substr_right(str, 14);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(sub, "ame was George"), 0);
    gds_str_destroy(sub);

    sub = gds_str_substr_right(str, 21);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(sub, "Whose name was George"), 0);
    gds_str_destroy(sub);

    /*  Test with number of characters greater than length  */
    
    sub = gds_str_substr_right(str, 100);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(sub, "Whose name was George"), 0);
    gds_str_destroy(sub);

    /*  Test when original string is empty  */

    gds_str_assign_cstr(str, "");
    sub = gds_str_substr_right(str, 100);
    TEST_ASSERT_EQUAL(gds_str_compare_cstr(sub, ""), 0);
    gds_str_destroy(sub);

    gds_str_destroy(str);
}

TEST_CASE(test_string_split)
{
    GDSString str = gds_str_create("key&value");
    GDSString left, right;

    /*  Test basic operation  */

    gds_str_split(str, &left, &right, '&');
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "key&value");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(left), "key");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(right), "value");
    gds_str_destroy(left);
    gds_str_destroy(right);

    /*  Test missing right value  */

    gds_str_assign_cstr(str, "key&");
    gds_str_split(str, &left, &right, '&');
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "key&");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(left), "key");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(right), "");
    gds_str_destroy(left);
    gds_str_destroy(right);

    /*  Test missing left value  */

    gds_str_assign_cstr(str, "&value");
    gds_str_split(str, &left, &right, '&');
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "&value");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(left), "");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(right), "value");
    gds_str_destroy(left);
    gds_str_destroy(right);

    /*  Test both values missing  */

    gds_str_assign_cstr(str, "&");
    gds_str_split(str, &left, &right, '&');
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "&");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(left), "");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(right), "");
    gds_str_destroy(left);
    gds_str_destroy(right);

    /*  Test empty string  */

    gds_str_assign_cstr(str, "");
    gds_str_split(str, &left, &right, '&');
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(left), "");
    TEST_ASSERT_EQUAL(right, NULL);
    gds_str_destroy(left);

    /*  Test missing delimiter  */

    gds_str_assign_cstr(str, "i'm just a string");
    gds_str_split(str, &left, &right, '&');
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "i'm just a string");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(left), "i'm just a string");
    TEST_ASSERT_EQUAL(right, NULL);
    gds_str_destroy(left);

    /*  Test NUL delimiter  */

    gds_str_assign_cstr(str, "i'm just a string");
    gds_str_split(str, &left, &right, 0);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "i'm just a string");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(left), "i'm just a string");
    TEST_ASSERT_EQUAL(right, NULL);
    gds_str_destroy(left);

    /*  Test NUL delimiter with empty string  */

    gds_str_assign_cstr(str, "");
    gds_str_split(str, &left, &right, 0);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "");
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(left), "");
    TEST_ASSERT_EQUAL(right, NULL);
    gds_str_destroy(left);

    gds_str_destroy(str);
}

TEST_CASE(test_string_trim_leading)
{
    GDSString str = gds_str_create("word");

    /*  Test with no leading whitespace  */

    gds_str_trim_leading(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "word");

    /*  Test with leading whitespace  */

    gds_str_assign_cstr(str, "  fish");
    gds_str_trim_leading(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "fish");

    /*  Test with leading and trailing whitespace  */

    gds_str_assign_cstr(str, "  dogs  ");
    gds_str_trim_leading(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "dogs  ");

    /*  Test with internal whitespace  */

    gds_str_assign_cstr(str, "  cats and cheese  ");
    gds_str_trim_leading(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "cats and cheese  ");

    /*  Test with other whitespace characters  */

    gds_str_assign_cstr(str, "\r\n \tcattle\t \r\n ");
    gds_str_trim_leading(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "cattle\t \r\n ");

    /*  Test with only whitespace characters  */

    gds_str_assign_cstr(str, "\r\n \t ");
    gds_str_trim_leading(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "");

    /*  Test with empty string  */

    gds_str_assign_cstr(str, "");
    gds_str_trim_leading(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "");

    gds_str_destroy(str);
}

TEST_CASE(test_string_trim_trailing)
{
    GDSString str = gds_str_create("word");

    /*  Test with no trailing whitespace  */

    gds_str_trim_trailing(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "word");

    /*  Test with trailing whitespace  */

    gds_str_assign_cstr(str, "fish  ");
    gds_str_trim_trailing(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "fish");

    /*  Test with leading and trailing whitespace  */

    gds_str_assign_cstr(str, "  dogs  ");
    gds_str_trim_trailing(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "  dogs");

    /*  Test with internal whitespace  */

    gds_str_assign_cstr(str, "  cats and cheese  ");
    gds_str_trim_trailing(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "  cats and cheese");

    /*  Test with other whitespace characters  */

    gds_str_assign_cstr(str, "\r\n \tcattle\t \r\n ");
    gds_str_trim_trailing(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "\r\n \tcattle");

    /*  Test with only whitespace characters  */

    gds_str_assign_cstr(str, "\r\n \t ");
    gds_str_trim_trailing(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "");

    /*  Test with empty string  */

    gds_str_assign_cstr(str, "");
    gds_str_trim_trailing(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "");

    gds_str_destroy(str);
}

TEST_CASE(test_string_trim)
{
    GDSString str = gds_str_create("word");

    /*  Test with no whitespace  */

    gds_str_trim(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "word");

    /*  Test with leading whitespace  */

    gds_str_assign_cstr(str, "  fish");
    gds_str_trim(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "fish");

    /*  Test with trailing whitespace  */

    gds_str_assign_cstr(str, "potato  ");
    gds_str_trim(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "potato");

    /*  Test with leading and trailing whitespace  */

    gds_str_assign_cstr(str, "  dogs  ");
    gds_str_trim(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "dogs");

    /*  Test with internal whitespace  */

    gds_str_assign_cstr(str, "  cats and cheese  ");
    gds_str_trim(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "cats and cheese");

    /*  Test with other whitespace characters  */

    gds_str_assign_cstr(str, "\r\n \tcattle\t \r\n ");
    gds_str_trim(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "cattle");

    /*  Test with only whitespace characters  */

    gds_str_assign_cstr(str, "\r\n \t ");
    gds_str_trim(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "");

    /*  Test with empty string  */

    gds_str_assign_cstr(str, "");
    gds_str_trim(str);
    TEST_ASSERT_STR_EQUAL(gds_str_cstr(str), "");

    gds_str_destroy(str);
}

TEST_CASE(test_string_char_at_index)
{
    GDSString str = gds_str_create("hello world!\n");

    TEST_ASSERT_EQUAL(gds_str_char_at_index(str, 0), 'h');
    TEST_ASSERT_EQUAL(gds_str_char_at_index(str, 1), 'e');
    TEST_ASSERT_EQUAL(gds_str_char_at_index(str, 2), 'l');
    TEST_ASSERT_EQUAL(gds_str_char_at_index(str, 3), 'l');
    TEST_ASSERT_EQUAL(gds_str_char_at_index(str, 4), 'o');
    TEST_ASSERT_EQUAL(gds_str_char_at_index(str, 5), ' ');
    TEST_ASSERT_EQUAL(gds_str_char_at_index(str, 6), 'w');
    TEST_ASSERT_EQUAL(gds_str_char_at_index(str, 7), 'o');
    TEST_ASSERT_EQUAL(gds_str_char_at_index(str, 8), 'r');
    TEST_ASSERT_EQUAL(gds_str_char_at_index(str, 9), 'l');
    TEST_ASSERT_EQUAL(gds_str_char_at_index(str, 10), 'd');
    TEST_ASSERT_EQUAL(gds_str_char_at_index(str, 11), '!');
    TEST_ASSERT_EQUAL(gds_str_char_at_index(str, 12), '\n');

    gds_str_destroy(str);
}

TEST_CASE(test_string_is_alnum)
{
    GDSString str = gds_str_create("");

    TEST_ASSERT_FALSE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, " ");
    TEST_ASSERT_FALSE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, "a");
    TEST_ASSERT_TRUE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, "a ");
    TEST_ASSERT_FALSE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, " a");
    TEST_ASSERT_FALSE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, "1");
    TEST_ASSERT_TRUE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, "abc");
    TEST_ASSERT_TRUE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, "123");
    TEST_ASSERT_TRUE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, "abc123");
    TEST_ASSERT_TRUE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, "123abc");
    TEST_ASSERT_TRUE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, "123abc ");
    TEST_ASSERT_FALSE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, " 123abc");
    TEST_ASSERT_FALSE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, " 123abc ");
    TEST_ASSERT_FALSE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, "?123abc.");
    TEST_ASSERT_FALSE(gds_str_is_alnum(str));

    gds_str_assign_cstr(str, "$123%abc9");
    TEST_ASSERT_FALSE(gds_str_is_alnum(str));

    gds_str_destroy(str);
}

TEST_CASE(test_string_intval)
{
    GDSString str = gds_str_create("123");
    int n;

    /*  Check basic operation  */

    TEST_ASSERT_TRUE(gds_str_intval(str, 0, &n));
    TEST_ASSERT_EQUAL(n, 123);

    /*  Test failure with bad character  */

    gds_str_assign_cstr(str, "123?");
    TEST_ASSERT_FALSE(gds_str_intval(str, 0, &n));

    /*  Test leading whitespace is OK (follows strtol())  */

    gds_str_assign_cstr(str, "    4756");
    TEST_ASSERT_TRUE(gds_str_intval(str, 0, &n));
    TEST_ASSERT_EQUAL(n, 4756);

    /*  Test trailing whitespace is NOT OK (*endptr is not 0)  */

    gds_str_assign_cstr(str, "    4756     ");
    TEST_ASSERT_FALSE(gds_str_intval(str, 0, &n));

    /*  Test binary conversion  */

    gds_str_assign_cstr(str, "100101");
    TEST_ASSERT_TRUE(gds_str_intval(str, 2, &n));
    TEST_ASSERT_EQUAL(n, 37);

    /*  Test hex conversion  */

    gds_str_assign_cstr(str, "FFA9C");
    TEST_ASSERT_TRUE(gds_str_intval(str, 16, &n));
    TEST_ASSERT_EQUAL(n, 1047196);

    /*  Test failure on hex conversion with decimal specified  */

    gds_str_assign_cstr(str, "FFA9C");
    TEST_ASSERT_FALSE(gds_str_intval(str, 10, &n));

    gds_str_destroy(str);
}

TEST_CASE(test_string_doubleval)
{
    GDSString str = gds_str_create("123");
    double d;

    /*  Check basic operation  */

    TEST_ASSERT_TRUE(gds_str_doubleval(str, &d));
    TEST_ASSERT_ALMOST_EQUAL(d, 123.0, 0.0001);

    gds_str_assign_cstr(str, "123456.7890123");
    TEST_ASSERT_TRUE(gds_str_doubleval(str, &d));
    TEST_ASSERT_ALMOST_EQUAL(d, 123456.7890123L, 0.00000001);

    gds_str_assign_cstr(str, "-3.14159e10");
    TEST_ASSERT_TRUE(gds_str_doubleval(str, &d));
    TEST_ASSERT_ALMOST_EQUAL(d, -31415900000L, 0.00000001);

    gds_str_destroy(str);
}

void test_string(void)
{
    RUN_CASE(test_string_create_simple);
    RUN_CASE(test_string_create_empty);
    RUN_CASE(test_string_compare);
    RUN_CASE(test_string_compare_cstring);
    RUN_CASE(test_string_dup);
    RUN_CASE(test_string_create_sprintf);
    RUN_CASE(test_string_assign);
    RUN_CASE(test_string_assign_empty);
    RUN_CASE(test_string_assign_cstr);
    RUN_CASE(test_string_assign_cstr_empty);
    RUN_CASE(test_string_concat);
    RUN_CASE(test_string_concat_empty);
    RUN_CASE(test_string_concat_cstr);
    RUN_CASE(test_string_concat_cstr_empty);
    RUN_CASE(test_string_trunc);
    RUN_CASE(test_string_strchr);
    RUN_CASE(test_string_substr_left);
    RUN_CASE(test_string_substr_right);
    RUN_CASE(test_string_split);
    RUN_CASE(test_string_trim_leading);
    RUN_CASE(test_string_trim_trailing);
    RUN_CASE(test_string_trim);
    RUN_CASE(test_string_char_at_index);
    RUN_CASE(test_string_is_alnum);
    RUN_CASE(test_string_intval);
    RUN_CASE(test_string_doubleval);
}
