/*  Unit tests for generic stack data structure  */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "test_stack.h"
#include "test_logging.h"

TEST_SUITE(test_stack);

/*  Test basic stack operations  */

TEST_CASE(test_stack_basic_ops)
{
    Stack stk = stack_create(3, DATATYPE_INT, 0);
    if ( !stk ) {
        perror("couldn't create stack");
        exit(EXIT_FAILURE);
    }

    int n;
        
    /*  Check stack attributes when created and empty  */

    TEST_ASSERT_EQUAL(stack_capacity(stk), 3);
    TEST_ASSERT_EQUAL(stack_size(stk), 0);
    TEST_ASSERT_EQUAL(stack_free_space(stk), 3);
    TEST_ASSERT_TRUE(stack_is_empty(stk));
    TEST_ASSERT_FALSE(stack_is_full(stk));

    /*  Check no pop from an empty stack  */

    TEST_ASSERT_FALSE(stack_pop(stk, &n));

    /*  Check pushing three items onto stack  */

    TEST_ASSERT_TRUE(stack_push(stk, 1));
    TEST_ASSERT_TRUE(stack_push(stk, -2));
    TEST_ASSERT_TRUE(stack_push(stk, 5));

    /*  Check no push onto a full stack  */

    TEST_ASSERT_FALSE(stack_push(stk, 7));

    /*  Check peek at top of stack  */

    TEST_ASSERT_TRUE(stack_peek(stk, &n));
    TEST_ASSERT_EQUAL(n, 5);

    /*  Check stack attributes when full  */

    TEST_ASSERT_EQUAL(stack_capacity(stk), 3);
    TEST_ASSERT_EQUAL(stack_size(stk), 3);
    TEST_ASSERT_EQUAL(stack_free_space(stk), 0);
    TEST_ASSERT_FALSE(stack_is_empty(stk));
    TEST_ASSERT_TRUE(stack_is_full(stk));

    /*  Check some pops and a peek  */

    TEST_ASSERT_TRUE(stack_pop(stk, &n));
    TEST_ASSERT_EQUAL(n, 5);

    TEST_ASSERT_TRUE(stack_pop(stk, &n));
    TEST_ASSERT_EQUAL(n, -2);

    TEST_ASSERT_TRUE(stack_peek(stk, &n));
    TEST_ASSERT_EQUAL(n, 1);

    /*  Check stack attributes when partially full  */

    TEST_ASSERT_EQUAL(stack_capacity(stk), 3);
    TEST_ASSERT_EQUAL(stack_size(stk), 1);
    TEST_ASSERT_EQUAL(stack_free_space(stk), 2);
    TEST_ASSERT_FALSE(stack_is_empty(stk));
    TEST_ASSERT_FALSE(stack_is_full(stk));

    /*  Check pop last element  */

    TEST_ASSERT_TRUE(stack_pop(stk, &n));
    TEST_ASSERT_EQUAL(n, 1);

    /*  Check no pop from an emptied stack  */

    TEST_ASSERT_FALSE(stack_pop(stk, &n));

    /*  Check stack attributes when empty again  */

    TEST_ASSERT_EQUAL(stack_capacity(stk), 3);
    TEST_ASSERT_EQUAL(stack_size(stk), 0);
    TEST_ASSERT_EQUAL(stack_free_space(stk), 3);
    TEST_ASSERT_TRUE(stack_is_empty(stk));
    TEST_ASSERT_FALSE(stack_is_full(stk));

    stack_destroy(stk);
}

/*  Test that dynamically allocated strings are appropriately
 *  freed when a stack is destroyed. Run this test case through
 *  Valgrind or a similar tool to check no memory leaks.         */

TEST_CASE(test_stack_free_strings)
{
    Stack stack = stack_create(1, DATATYPE_STRING,
                               GDS_RESIZABLE | GDS_FREE_ON_DESTROY);
    if ( !stack ) {
        perror("couldn't create stack");
        exit(EXIT_FAILURE);
    }

    /*  Push strings  */

    TEST_ASSERT_TRUE(stack_push(stack, strdup("First string")));
    TEST_ASSERT_TRUE(stack_push(stack, strdup("Second string")));
    TEST_ASSERT_TRUE(stack_push(stack, strdup("Third string")));

    /*  Test stack attributes  */

    TEST_ASSERT_EQUAL(stack_size(stack), 3);
    TEST_ASSERT_EQUAL(stack_capacity(stack), 4);
    TEST_ASSERT_EQUAL(stack_free_space(stack), 1);
    TEST_ASSERT_FALSE(stack_is_empty(stack));
    TEST_ASSERT_FALSE(stack_is_full(stack));
    
    /*  Destroy stack while it contains dynamically allocated elements  */

    stack_destroy(stack);
}

void test_stack(void)
{
    RUN_CASE(test_stack_basic_ops);
    RUN_CASE(test_stack_free_strings);
}
