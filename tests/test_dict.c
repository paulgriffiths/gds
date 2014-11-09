#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"
#include "test_dict.h"
#include "test_logging.h"

static void test_dict_one(void)
{
    Dict dict = dict_create(DATATYPE_INT, 0);
    if ( !dict ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    bool status;

    status = dict_insert(dict, "meaning", 42);
    tests_log_test(status, "dict_insert() failed");
    
    status = dict_insert(dict, "hastings", 1066);
    tests_log_test(status, "dict_insert() failed");
    
    status = dict_insert(dict, "dwarves", 7);
    tests_log_test(status, "dict_insert() failed");
    
    status = dict_has_key(dict, "meaning");
    tests_log_test(status, "dict_has_key() failed to pick up key");
    
    status = dict_has_key(dict, "nonsense");
    tests_log_test(!status, "dict_has_key() incorrectly picked up key");
    
    status = dict_has_key(dict, "hastings");
    tests_log_test(status, "dict_has_key() failed to pick up key");
    
    status = dict_has_key(dict, "absent");
    tests_log_test(!status, "dict_has_key() incorrectly picked up key");
    
    status = dict_has_key(dict, "dwarves");
    tests_log_test(status, "dict_has_key() failed to pick up key");
    
    status = dict_has_key(dict, "there's none");
    tests_log_test(!status, "dict_has_key() incorrectly picked up key");
    
    dict_destroy(dict);
}

void test_dict(void)
{
    test_dict_one();
}
