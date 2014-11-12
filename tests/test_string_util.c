#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_util.h"
#include "test_string_util.h"
#include "test_logging.h"

void test_trim_left(void)
{
    char control[20];
    
    strcpy(control, "");
    gds_trim_left(control);
    tests_log_test(!strcmp(control, ""), "gds_trim_left() failed");

    strcpy(control, "   ");
    gds_trim_left(control);
    tests_log_test(!strcmp(control, ""), "gds_trim_left() failed");

    strcpy(control, "two words");
    gds_trim_left(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim_left() failed");

    strcpy(control, "   two words");
    gds_trim_left(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim_left() failed");

    strcpy(control, "two words  ");
    gds_trim_left(control);
    tests_log_test(!strcmp(control, "two words  "), "gds_trim_left() failed");

    strcpy(control, "   two words  ");
    gds_trim_left(control);
    tests_log_test(!strcmp(control, "two words  "), "gds_trim_left() failed");
    
    strcpy(control, "\t   two words");
    gds_trim_left(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim_left() failed");
    
    strcpy(control, "\n   two words");
    gds_trim_left(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim_left() failed");
    
    strcpy(control, "\r   two words");
    gds_trim_left(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim_left() failed");
}

void test_trim_right(void)
{
    char control[20];
    
    strcpy(control, "");
    gds_trim_right(control);
    tests_log_test(!strcmp(control, ""), "gds_trim_right() failed");

    strcpy(control, "   ");
    gds_trim_right(control);
    tests_log_test(!strcmp(control, ""), "gds_trim_right() failed");

    strcpy(control, "two words");
    gds_trim_right(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim_right() failed");

    strcpy(control, "   two words");
    gds_trim_right(control);
    tests_log_test(!strcmp(control, "   two words"), "gds_trim_right() failed");

    strcpy(control, "two words  ");
    gds_trim_right(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim_right() failed");

    strcpy(control, "   two words  ");
    gds_trim_right(control);
    tests_log_test(!strcmp(control, "   two words"), "gds_trim_right() failed");
    
    strcpy(control, "two words  \t");
    gds_trim_right(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim_right() failed");
    
    strcpy(control, "two words  \n");
    gds_trim_right(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim_right() failed");
    
    strcpy(control, "two words  \r");
    gds_trim_right(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim_right() failed");
}

void test_trim(void)
{
    char control[20];
    
    strcpy(control, "");
    gds_trim(control);
    tests_log_test(!strcmp(control, ""), "gds_trim() failed");

    strcpy(control, "   ");
    gds_trim(control);
    tests_log_test(!strcmp(control, ""), "gds_trim() failed");

    strcpy(control, "two words");
    gds_trim(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim() failed");

    strcpy(control, "   two words");
    gds_trim(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim() failed");

    strcpy(control, "two words  ");
    gds_trim(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim() failed");

    strcpy(control, "   two words  ");
    gds_trim(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim() failed");
    
    strcpy(control, "two words  \t");
    gds_trim(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim() failed");
    
    strcpy(control, "two words  \n");
    gds_trim(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim() failed");
    
    strcpy(control, "two words  \r");
    gds_trim(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim() failed");

    strcpy(control, "\t   two words");
    gds_trim(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim() failed");
    
    strcpy(control, "\n   two words");
    gds_trim(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim() failed");
    
    strcpy(control, "\r   two words");
    gds_trim(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim() failed");

    strcpy(control, "\t   two words  \n");
    gds_trim(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim() failed");
    
    strcpy(control, "\n   two words  \r");
    gds_trim(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim() failed");
    
    strcpy(control, "\r   two words  \n");
    gds_trim(control);
    tests_log_test(!strcmp(control, "two words"), "gds_trim() failed");
}

void test_pair_string(void)
{
    struct pair_string * ps = pair_string_create("key=value", '=');
    if ( !ps ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(!strcmp(ps->first, "key"),
                   "pair_string_create() failed");
    tests_log_test(!strcmp(ps->second, "value"),
                   "pair_string_create() failed");
    pair_string_destroy(ps);

    ps = pair_string_create("  chalk/cheese  ", '/');
    if ( !ps ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(!strcmp(ps->first, "  chalk"),
                   "pair_string_create() failed");
    tests_log_test(!strcmp(ps->second, "cheese  "),
                   "pair_string_create() failed");
    pair_string_destroy(ps);

    ps = pair_string_create(" apples   &   pairs ", '&');
    if ( !ps ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(!strcmp(ps->first, " apples   "),
                   "pair_string_create() failed");
    tests_log_test(!strcmp(ps->second, "   pairs "),
                   "pair_string_create() failed");
    pair_string_destroy(ps);

    ps = pair_string_create("one=two=three", '=');
    if ( !ps ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(!strcmp(ps->first, "one"),
                   "pair_string_create() failed");
    tests_log_test(!strcmp(ps->second, "two=three"),
                   "pair_string_create() failed");
    pair_string_destroy(ps);

    ps = pair_string_create(" spaces :   ", ':');
    if ( !ps ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(!strcmp(ps->first, " spaces "),
                   "pair_string_create() failed");
    tests_log_test(!strcmp(ps->second, "   "),
                   "pair_string_create() failed");
    pair_string_destroy(ps);

    ps = pair_string_create("empty-", '-');
    if ( !ps ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(!strcmp(ps->first, "empty"),
                   "pair_string_create() failed");
    tests_log_test(!strcmp(ps->second, ""),
                   "pair_string_create() failed");
    pair_string_destroy(ps);

    ps = pair_string_create("reallyempty", '=');
    if ( !ps ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(!strcmp(ps->first, "reallyempty"),
                   "pair_string_create() failed");
    tests_log_test(!strcmp(ps->second, ""),
                   "pair_string_create() failed");
    pair_string_destroy(ps);

    ps = pair_string_create("", '=');
    if ( !ps ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(!strcmp(ps->first, ""),
                   "pair_string_create() failed");
    tests_log_test(!strcmp(ps->second, ""),
                   "pair_string_create() failed");
    pair_string_destroy(ps);

    ps = pair_string_create("=", '=');
    if ( !ps ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(!strcmp(ps->first, ""),
                   "pair_string_create() failed");
    tests_log_test(!strcmp(ps->second, ""),
                   "pair_string_create() failed");
    pair_string_destroy(ps);
}

void test_pair_string_copy(void)
{
    struct pair_string * ps = pair_string_create("key=value", '=');
    if ( !ps ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    struct pair_string * psc = pair_string_copy(ps);
    if ( !psc ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    pair_string_destroy(ps);

    tests_log_test(!strcmp(psc->first, "key"),
                   "pair_string_copy() failed");
    tests_log_test(!strcmp(psc->second, "value"),
                   "pair_string_copy() failed");

    pair_string_destroy(psc);

    ps = pair_string_create("key=", '=');
    if ( !ps ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    psc = pair_string_copy(ps);
    if ( !psc ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    pair_string_destroy(ps);

    tests_log_test(!strcmp(psc->first, "key"),
                   "pair_string_copy() failed");
    tests_log_test(!strcmp(psc->second, ""),
                   "pair_string_copy() failed");

    pair_string_destroy(psc);

    ps = pair_string_create("", '=');
    if ( !ps ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    psc = pair_string_copy(ps);
    if ( !psc ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    pair_string_destroy(ps);

    tests_log_test(!strcmp(psc->first, ""),
                   "pair_string_copy() failed");
    tests_log_test(!strcmp(psc->second, ""),
                   "pair_string_copy() failed");

    pair_string_destroy(psc);

}

void test_split_string(void)
{
    struct list_string * list = split_string("one,two,three,four", ',');
    if ( !list ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(list->size == 4, "split_string() gave wrong size");
    tests_log_test(!strcmp(list->list[0], "one"),
                   "split_string() gave wrong string");
    tests_log_test(!strcmp(list->list[1], "two"),
                   "split_string() gave wrong string");
    tests_log_test(!strcmp(list->list[2], "three"),
                   "split_string() gave wrong string");
    tests_log_test(!strcmp(list->list[3], "four"),
                   "split_string() gave wrong string");
    tests_log_test(list->list[4] == NULL,
                   "split_string() didn't NULL terminate");

    list_string_destroy(list);

    list = split_string("", ':');
    if ( !list ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(list->size == 0, "split_string() gave wrong size");
    tests_log_test(list->list[0] == NULL,
                   "split_string() didn't NULL terminate");

    list_string_destroy(list);

    list = split_string("single", '/');
    if ( !list ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(list->size == 1, "split_string() gave wrong size");
    tests_log_test(!strcmp(list->list[0], "single"),
                   "split_string() gave wrong string");
    tests_log_test(list->list[1] == NULL,
                   "split_string() didn't NULL terminate");

    list_string_destroy(list);

    list = split_string("singlewithdelim&", '&');
    if ( !list ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(list->size == 1, "split_string() gave wrong size");
    tests_log_test(!strcmp(list->list[0], "singlewithdelim"),
                   "split_string() gave wrong string");
    tests_log_test(list->list[1] == NULL,
                   "split_string() didn't NULL terminate");

    list_string_destroy(list);

    list = split_string("three&with&delim&", '&');
    if ( !list ) {
        perror("couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    tests_log_test(list->size == 3, "split_string() gave wrong size");
    tests_log_test(!strcmp(list->list[0], "three"),
                   "split_string() gave wrong string");
    tests_log_test(!strcmp(list->list[1], "with"),
                   "split_string() gave wrong string");
    tests_log_test(!strcmp(list->list[2], "delim"),
                   "split_string() gave wrong string");
    tests_log_test(list->list[3] == NULL,
                   "split_string() didn't NULL terminate");

    list_string_destroy(list);
}

void test_string_util(void)
{
    test_trim_left();
    test_trim_right();
    test_trim();
    test_pair_string();
    test_pair_string_copy();
    test_split_string();
}
