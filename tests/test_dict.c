/*  Unit tests for generic dictionary data structure  */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pggds/dict.h>
#include <pggds/unittest.h>
#include <pggds/string_util.h>
#include "test_dict.h"

TEST_SUITE(test_dict);

TEST_CASE(test_dict_insert_int)
{
    Dict dict = dict_create(DATATYPE_INT, 0);
    if ( !dict ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    int n;

    TEST_ASSERT_TRUE(dict_insert(dict, "meaning", 42));
    TEST_ASSERT_TRUE(dict_insert(dict, "hastings", 1066));
    TEST_ASSERT_TRUE(dict_insert(dict, "dwarves", 7));
    
    TEST_ASSERT_TRUE(dict_has_key(dict, "meaning"));
    TEST_ASSERT_TRUE(dict_value_for_key(dict, "meaning", &n));
    TEST_ASSERT_EQUAL(n, 42);

    TEST_ASSERT_FALSE(dict_has_key(dict, "nonsense"));
    TEST_ASSERT_FALSE(dict_value_for_key(dict, "nonsense", &n));

    TEST_ASSERT_TRUE(dict_has_key(dict, "hastings"));
    TEST_ASSERT_TRUE(dict_value_for_key(dict, "hastings", &n));
    TEST_ASSERT_EQUAL(n, 1066);

    TEST_ASSERT_FALSE(dict_has_key(dict, "absent"));
    TEST_ASSERT_FALSE(dict_value_for_key(dict, "absent", &n));

    TEST_ASSERT_TRUE(dict_has_key(dict, "dwarves"));
    TEST_ASSERT_TRUE(dict_value_for_key(dict, "dwarves", &n));
    TEST_ASSERT_EQUAL(n, 7);

    TEST_ASSERT_FALSE(dict_has_key(dict, "there's none"));
    TEST_ASSERT_FALSE(dict_value_for_key(dict, "there's none", &n));

    /*  A dwarf died, so overwrite the value to check the old frees  */

    TEST_ASSERT_TRUE(dict_insert(dict, "dwarves", 6));

    TEST_ASSERT_TRUE(dict_has_key(dict, "dwarves"));
    TEST_ASSERT_TRUE(dict_value_for_key(dict, "dwarves", &n));
    TEST_ASSERT_EQUAL(n, 6);

    dict_destroy(dict);
}

TEST_CASE(test_dict_insert_string)
{
    Dict dict = dict_create(DATATYPE_STRING, GDS_FREE_ON_DESTROY);
    if ( !dict ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    char * pc;

    TEST_ASSERT_TRUE(dict_insert(dict, "meaning", gds_strdup("42")));
    TEST_ASSERT_TRUE(dict_insert(dict, "hastings", gds_strdup("1066")));
    TEST_ASSERT_TRUE(dict_insert(dict, "dwarves", gds_strdup("7")));
    
    TEST_ASSERT_TRUE(dict_has_key(dict, "meaning"));
    TEST_ASSERT_TRUE(dict_value_for_key(dict, "meaning", &pc));
    TEST_ASSERT_STR_EQUAL(pc, "42");

    TEST_ASSERT_FALSE(dict_has_key(dict, "nonsense"));
    TEST_ASSERT_FALSE(dict_value_for_key(dict, "nonsense", &pc));

    TEST_ASSERT_TRUE(dict_has_key(dict, "hastings"));
    TEST_ASSERT_TRUE(dict_value_for_key(dict, "hastings", &pc));
    TEST_ASSERT_STR_EQUAL(pc, "1066");

    TEST_ASSERT_FALSE(dict_has_key(dict, "absent"));
    TEST_ASSERT_FALSE(dict_value_for_key(dict, "absent", &pc));

    TEST_ASSERT_TRUE(dict_has_key(dict, "dwarves"));
    TEST_ASSERT_TRUE(dict_value_for_key(dict, "dwarves", &pc));
    TEST_ASSERT_STR_EQUAL(pc, "7");

    TEST_ASSERT_FALSE(dict_has_key(dict, "there's none"));
    TEST_ASSERT_FALSE(dict_value_for_key(dict, "there's none", &pc));

    /*  A dwarf died, so overwrite the value to check the old frees  */

    TEST_ASSERT_TRUE(dict_insert(dict, "dwarves", gds_strdup("6")));

    TEST_ASSERT_TRUE(dict_has_key(dict, "dwarves"));
    TEST_ASSERT_TRUE(dict_value_for_key(dict, "dwarves", &pc));
    TEST_ASSERT_STR_EQUAL(pc, "6");

    dict_destroy(dict);
}

/*  Test key deletion  */

TEST_CASE(test_dict_delete)
{
    Dict dict = dict_create(DATATYPE_STRING, GDS_FREE_ON_DESTROY);
    if ( !dict ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    char * pc;

    /*  Insert some keys  */

    TEST_ASSERT_TRUE(dict_insert(dict, "john", gds_strdup("Bolton")));
    TEST_ASSERT_TRUE(dict_insert(dict, "mary", gds_strdup("Portsmouth")));
    TEST_ASSERT_TRUE(dict_insert(dict, "skeletor", gds_strdup("Hull")));

    /*  Check the dictionary is as expected  */

    TEST_ASSERT_TRUE(dict_has_key(dict, "john"));
    TEST_ASSERT_TRUE(dict_has_key(dict, "mary"));
    TEST_ASSERT_TRUE(dict_has_key(dict, "skeletor"));

    TEST_ASSERT_TRUE(dict_value_for_key(dict, "john", &pc));
    TEST_ASSERT_STR_EQUAL(pc, "Bolton");
    TEST_ASSERT_TRUE(dict_value_for_key(dict, "mary", &pc));
    TEST_ASSERT_STR_EQUAL(pc, "Portsmouth");
    TEST_ASSERT_TRUE(dict_value_for_key(dict, "skeletor", &pc));
    TEST_ASSERT_STR_EQUAL(pc, "Hull");

    /*  Delete a key  */

    TEST_ASSERT_TRUE(dict_delete(dict, "skeletor"));
    TEST_ASSERT_FALSE(dict_delete(dict, "fabio"));

    /*  Check the dictionary is as expected  */

    TEST_ASSERT_TRUE(dict_has_key(dict, "john"));
    TEST_ASSERT_TRUE(dict_has_key(dict, "mary"));
    TEST_ASSERT_FALSE(dict_has_key(dict, "skeletor"));

    TEST_ASSERT_TRUE(dict_value_for_key(dict, "john", &pc));
    TEST_ASSERT_STR_EQUAL(pc, "Bolton");
    TEST_ASSERT_TRUE(dict_value_for_key(dict, "mary", &pc));
    TEST_ASSERT_STR_EQUAL(pc, "Portsmouth");

    dict_destroy(dict);
}

void test_dict(void)
{
    RUN_CASE(test_dict_insert_int);
    RUN_CASE(test_dict_insert_string);
    RUN_CASE(test_dict_delete);
}
