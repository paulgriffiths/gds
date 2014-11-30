#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pggds/unittest.h>
#include "test_stack.h"
#include "test_queue.h"
#include "test_list.h"
#include "test_vector.h"
#include "test_dict.h"
#include "test_string.h"
#include "test_string_util.h"
#include "test_std_wrappers.h"

int main(int argc, char ** argv)
{
    bool stack = false, queue = false, list = false, vector = false;
    bool dict = false, string_util = false, gds_string = false;
    bool stdwrap = false;

    if ( argc < 2 ) {
        stack = true;
        queue = true;
        list = true;
        vector = true;
        dict = true;
        string_util = true;
        gds_string = true;
        stdwrap = true;
    }
    else {
        size_t i = 0;

        while ( argv[++i] ) {
            if ( !strcmp(argv[i], "stack") ) {
                stack = true;
            }
            else if ( !strcmp(argv[i], "queue") ) {
                queue = true;
            }
            else if ( !strcmp(argv[i], "list") ) {
                list = true;
            }
            else if ( !strcmp(argv[i], "vector") ) {
                vector = true;
            }
            else if ( !strcmp(argv[i], "dict") ) {
                dict = true;
            }
            else if ( !strcmp(argv[i], "string_util") ) {
                string_util = true;
            }
            else if ( !strcmp(argv[i], "string") ) {
                gds_string = true;
            }
            else if ( !strcmp(argv[i], "stdwrap") ) {
                stdwrap = true;
            }
        }
    }

    tests_initialize();

    if ( stack ) {
        printf("Running unit tests for generic stack...\n");
        test_stack();
    }

    if ( queue ) {
        printf("Running unit tests for generic queue...\n");
        test_queue();
    }

    if ( list ) {
        printf("Running unit tests for generic list...\n");
        test_list();
    }

    if ( vector ) {
        printf("Running unit tests for generic vector...\n");
        test_vector();
    }

    if ( dict ) {
        printf("Running unit tests for generic dictionary...\n");
        test_dict();
    }

    if ( gds_string ) {
        printf("Running unit tests for string...\n");
        test_string();
    }

    if ( string_util ) {
        printf("Running unit tests for string utilities...\n");
        test_string_util();
    }

    if ( stdwrap ) {
        printf("Running unit tests for standard wrappers...\n");
        test_std_wrappers();
    }

    tests_report();

    return 0;
}
