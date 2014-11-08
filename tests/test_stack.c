#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "test_stack.h"
#include "test_logging.h"

static void test_stack_one(void)
{
    Stack stk = stack_create(3, DATATYPE_INT, 0);

    int n;
    size_t sz;
    bool status;
        
    sz = stack_capacity(stk);
    tests_log_test(sz == 3, "stack_capacity() gave wrong value");

    sz = stack_size(stk);
    tests_log_test(sz == 0, "stack_size() gave wrong value");

    sz = stack_free_space(stk);
    tests_log_test(sz == 3, "stack_free_space() gave wrong value");

    status = stack_is_empty(stk);
    tests_log_test(status, "stack_is_empty() gave wrong value");

    status = stack_is_full(stk);
    tests_log_test(!status, "stack_is_full() gave wrong value");

    status = stack_pop(stk, &n);
    tests_log_test(!status, "stack_pop() improperly succeeded");

    status = stack_push(stk, 1);
    tests_log_test(status, "stack_push() failed");
    
    status = stack_push(stk, -2);
    tests_log_test(status, "stack_push() failed");

    status = stack_push(stk, 5);
    tests_log_test(status, "stack_push() failed");

    status = stack_push(stk, 7);
    tests_log_test(!status, "stack_push() improperly succeeded");

    status = stack_peek(stk, &n);
    tests_log_test(status, "stack_peek() failed");
    tests_log_test(n == 5, "stack_peek() gave wrong value");

    sz = stack_capacity(stk);
    tests_log_test(sz == 3, "stack_capacity() gave wrong value");

    sz = stack_size(stk);
    tests_log_test(sz == 3, "stack_size() gave wrong value");

    sz = stack_free_space(stk);
    tests_log_test(sz == 0, "stack_free_space() gave wrong value");

    status = stack_is_empty(stk);
    tests_log_test(!status, "stack_is_empty() gave wrong value");

    status = stack_is_full(stk);
    tests_log_test(status, "stack_is_full() gave wrong value");

    status = stack_pop(stk, &n);
    tests_log_test(status, "stack_pop() failed");
    tests_log_test(n == 5, "stack_pop() gave wrong value");

    status = stack_pop(stk, &n);
    tests_log_test(status, "stack_pop() failed");
    tests_log_test(n == -2, "stack_pop() gave wrong value");

    status = stack_peek(stk, &n);
    tests_log_test(status, "stack_peek() failed");
    tests_log_test(n == 1, "stack_peek() gave wrong value");

    sz = stack_capacity(stk);
    tests_log_test(sz == 3, "stack_capacity() gave wrong value");

    sz = stack_size(stk);
    tests_log_test(sz == 1, "stack_size() gave wrong value");

    sz = stack_free_space(stk);
    tests_log_test(sz == 2, "stack_free_space() gave wrong value");

    status = stack_is_empty(stk);
    tests_log_test(!status, "stack_is_empty() gave wrong value");

    status = stack_is_full(stk);
    tests_log_test(!status, "stack_is_full() gave wrong value");

    status = stack_pop(stk, &n);
    tests_log_test(status, "stack_pop() failed");
    tests_log_test(n == 1, "stack_pop() gave wrong value");

    sz = stack_capacity(stk);
    tests_log_test(sz == 3, "stack_capacity() gave wrong value");

    sz = stack_size(stk);
    tests_log_test(sz == 0, "stack_size() gave wrong value");

    sz = stack_free_space(stk);
    tests_log_test(sz == 3, "stack_free_space() gave wrong value");

    status = stack_is_empty(stk);
    tests_log_test(status, "stack_is_empty() gave wrong value");

    status = stack_is_full(stk);
    tests_log_test(!status, "stack_is_full() gave wrong value");

    stack_destroy(stk);
}

static void test_stack_free_strings(void)
{
    Stack stack = stack_create(1, DATATYPE_STRING,
                               GDS_RESIZABLE | GDS_FREE_ON_DESTROY);
    if ( !stack ) {
        perror("couldn't create stack");
        exit(EXIT_FAILURE);
    }

    bool status;
    size_t sz;

    status = stack_push(stack, strdup("First string"));
    tests_log_test(status, "stack_push() failed");

    status = stack_push(stack, strdup("Second string"));
    tests_log_test(status, "stack_push() failed");

    status = stack_push(stack, strdup("Third string"));
    tests_log_test(status, "stack_push() failed");

    sz = stack_size(stack);
    tests_log_test(sz == 3, "stack_length() gave wrong value");

    sz = stack_capacity(stack);
    tests_log_test(sz == 4, "stack_capacity() gave wrong value");

    sz = stack_free_space(stack);
    tests_log_test(sz == 1, "stack_free_space() gave wrong value");

    status = stack_is_empty(stack);
    tests_log_test(!status, "stack_is_empty() gave wrong value");
    
    stack_destroy(stack);
}

void test_stack(void)
{
    test_stack_one();
    test_stack_free_strings();
}
