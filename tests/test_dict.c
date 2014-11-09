#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"
#include "test_dict.h"
#include "test_logging.h"

static void test_dict_insert_int(void)
{
    Dict dict = dict_create(DATATYPE_INT, 0);
    if ( !dict ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    bool status;
    int n;

    status = dict_insert(dict, "meaning", 42);
    tests_log_test(status, "dict_insert() failed");
    
    status = dict_insert(dict, "hastings", 1066);
    tests_log_test(status, "dict_insert() failed");
    
    status = dict_insert(dict, "dwarves", 7);
    tests_log_test(status, "dict_insert() failed");
    
    status = dict_has_key(dict, "meaning");
    tests_log_test(status, "dict_has_key() failed to pick up key");
    
    status = dict_value_for_key(dict, "meaning", &n);
    tests_log_test(status, "dict_value_for_key() failed");
    tests_log_test(n == 42, "dict_value_for_key() gave wrong value");

    status = dict_has_key(dict, "nonsense");
    tests_log_test(!status, "dict_has_key() incorrectly picked up key");
    
    status = dict_value_for_key(dict, "nonsense", &n);
    tests_log_test(!status, "dict_value_for_key() incorrectly succeeded");

    status = dict_has_key(dict, "hastings");
    tests_log_test(status, "dict_has_key() failed to pick up key");
    
    status = dict_value_for_key(dict, "hastings", &n);
    tests_log_test(status, "dict_value_for_key() failed");
    tests_log_test(n == 1066, "dict_value_for_key() gave wrong value");

    status = dict_has_key(dict, "absent");
    tests_log_test(!status, "dict_has_key() incorrectly picked up key");
    
    status = dict_value_for_key(dict, "absent", &n);
    tests_log_test(!status, "dict_value_for_key() incorrectly succeeded");

    status = dict_has_key(dict, "dwarves");
    tests_log_test(status, "dict_has_key() failed to pick up key");
    
    status = dict_value_for_key(dict, "dwarves", &n);
    tests_log_test(status, "dict_value_for_key() failed");
    tests_log_test(n == 7, "dict_value_for_key() gave wrong value");

    status = dict_has_key(dict, "there's none");
    tests_log_test(!status, "dict_has_key() incorrectly picked up key");
    
    status = dict_value_for_key(dict, "there's none", &n);
    tests_log_test(!status, "dict_value_for_key() incorrectly succeeded");

    /*  A dwarf died, so overwrite the value to check the old frees  */

    status = dict_insert(dict, "dwarves", 6);
    tests_log_test(status, "dict_insert() failed");

    status = dict_has_key(dict, "dwarves");
    tests_log_test(status, "dict_has_key() failed to pick up key");
    
    status = dict_value_for_key(dict, "dwarves", &n);
    tests_log_test(status, "dict_value_for_key() failed");
    tests_log_test(6, "dict_value_for_key() gave wrong value");

    dict_destroy(dict);
}

static void test_dict_insert_string(void)
{
    Dict dict = dict_create(DATATYPE_STRING, GDS_FREE_ON_DESTROY);
    if ( !dict ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    bool status;
    char * pc;

    status = dict_insert(dict, "meaning", strdup("42"));
    tests_log_test(status, "dict_insert() failed");
    
    status = dict_insert(dict, "hastings", strdup("1066"));
    tests_log_test(status, "dict_insert() failed");
    
    status = dict_insert(dict, "dwarves", strdup("7"));
    tests_log_test(status, "dict_insert() failed");
    
    status = dict_has_key(dict, "meaning");
    tests_log_test(status, "dict_has_key() failed to pick up key");
    
    status = dict_value_for_key(dict, "meaning", &pc);
    tests_log_test(status, "dict_value_for_key() failed");
    tests_log_test(!strcmp("42", pc), "dict_value_for_key() gave wrong value");

    status = dict_has_key(dict, "nonsense");
    tests_log_test(!status, "dict_has_key() incorrectly picked up key");
    
    status = dict_value_for_key(dict, "nonsense", &pc);
    tests_log_test(!status, "dict_value_for_key() failed");

    status = dict_has_key(dict, "hastings");
    tests_log_test(status, "dict_has_key() failed to pick up key");
    
    status = dict_value_for_key(dict, "hastings", &pc);
    tests_log_test(status, "dict_value_for_key() failed");
    tests_log_test(!strcmp("1066", pc),
                   "dict_value_for_key() gave wrong value");

    status = dict_has_key(dict, "absent");
    tests_log_test(!status, "dict_has_key() incorrectly picked up key");
    
    status = dict_value_for_key(dict, "absent", &pc);
    tests_log_test(!status, "dict_value_for_key() failed");

    status = dict_has_key(dict, "dwarves");
    tests_log_test(status, "dict_has_key() failed to pick up key");
    
    status = dict_value_for_key(dict, "dwarves", &pc);
    tests_log_test(status, "dict_value_for_key() failed");
    tests_log_test(!strcmp("7", pc), "dict_value_for_key() gave wrong value");

    status = dict_has_key(dict, "there's none");
    tests_log_test(!status, "dict_has_key() incorrectly picked up key");
    
    status = dict_value_for_key(dict, "there's none", &pc);
    tests_log_test(!status, "dict_value_for_key() failed");

    /*  A dwarf died, so overwrite the value to check the old frees  */

    status = dict_insert(dict, "dwarves", strdup("6"));
    tests_log_test(status, "dict_insert() failed");

    status = dict_has_key(dict, "dwarves");
    tests_log_test(status, "dict_has_key() failed to pick up key");
    
    status = dict_value_for_key(dict, "dwarves", &pc);
    tests_log_test(status, "dict_value_for_key() failed");
    tests_log_test(!strcmp("6", pc), "dict_value_for_key() gave wrong value");

    dict_destroy(dict);
}

void test_dict(void)
{
    test_dict_insert_int();
    test_dict_insert_string();
}
