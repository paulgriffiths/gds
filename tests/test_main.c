#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "test_stack.h"
#include "test_queue.h"
#include "test_list.h"
#include "test_vector.h"
#include "test_logging.h"

int main(int argc, char ** argv)
{
    bool stack = false, queue = false, list = false, vector = false;

    if ( argc < 2 ) {
        stack = true;
        queue = true;
        list = true;
        vector = true;
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
        }
    }

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

    printf("%d successes and %d failures from %d tests.\n",
           tests_get_successes(), tests_get_failures(),
           tests_get_total_tests());

    return 0;
}
