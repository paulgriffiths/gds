#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "test_stack.h"
#include "test_queue.h"
#include "test_list.h"
#include "test_vector.h"
#include "test_dict.h"
#include "test_string_util.h"
#include "test_logging.h"

int main(int argc, char ** argv)
{
    bool stack = false, queue = false, list = false, vector = false;
    bool dict = false, string_util = false;

    if ( argc < 2 ) {
        stack = true;
        queue = true;
        list = true;
        vector = true;
        dict = true;
        string_util = true;
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

    if ( string_util ) {
        printf("Running unit tests for string utilities...\n");
        test_string_util();
    }

    tests_report();

    return 0;
}
