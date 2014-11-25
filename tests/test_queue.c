/*  Unit tests for generic queue data structure  */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "test_queue.h"
#include "test_logging.h"

TEST_SUITE(test_queue);

/*  Test basic queue operations  */

TEST_CASE(test_queue_basic_ops)
{
    Queue que = queue_create(3, DATATYPE_INT, 0);
    if ( !que ) {
        perror("couldn't allocate memory for queue");
        exit(EXIT_FAILURE);
    }

    int n;
        
    /*  Test queue attributes after creation  */

    TEST_ASSERT_EQUAL(queue_capacity(que), 3);
    TEST_ASSERT_EQUAL(queue_size(que), 0);
    TEST_ASSERT_EQUAL(queue_free_space(que), 3);
    TEST_ASSERT_TRUE(queue_is_empty(que));
    TEST_ASSERT_FALSE(queue_is_full(que));

    /*  Test no pop from empty queue  */

    TEST_ASSERT_FALSE(queue_pop(que, &n));

    /*  Test push elements  */

    TEST_ASSERT_TRUE(queue_push(que, 1));
    TEST_ASSERT_TRUE(queue_push(que, -2));
    TEST_ASSERT_TRUE(queue_push(que, 5));

    /*  Test no push onto full queue  */

    TEST_ASSERT_FALSE(queue_push(que, 7));

    /*  Test peek at front element  */

    TEST_ASSERT_TRUE(queue_peek(que, &n));
    TEST_ASSERT_EQUAL(n, 1);

    /*  Test queue attributes when full  */

    TEST_ASSERT_EQUAL(queue_capacity(que), 3);
    TEST_ASSERT_EQUAL(queue_size(que), 3);
    TEST_ASSERT_EQUAL(queue_free_space(que), 0);
    TEST_ASSERT_FALSE(queue_is_empty(que));
    TEST_ASSERT_TRUE(queue_is_full(que));

    /*  Test two pops and a peek  */

    TEST_ASSERT_TRUE(queue_pop(que, &n));
    TEST_ASSERT_EQUAL(n, 1);

    TEST_ASSERT_TRUE(queue_pop(que, &n));
    TEST_ASSERT_EQUAL(n, -2);

    TEST_ASSERT_TRUE(queue_peek(que, &n));
    TEST_ASSERT_EQUAL(n, 5);

    /*  Test queue attributes when partially full  */

    TEST_ASSERT_EQUAL(queue_capacity(que), 3);
    TEST_ASSERT_EQUAL(queue_size(que), 1);
    TEST_ASSERT_EQUAL(queue_free_space(que), 2);
    TEST_ASSERT_FALSE(queue_is_empty(que));
    TEST_ASSERT_FALSE(queue_is_full(que));

    /*  Test pop last element  */

    TEST_ASSERT_TRUE(queue_pop(que, &n));
    TEST_ASSERT_EQUAL(n, 5);

    /*  Test no pop from emptied queue  */

    TEST_ASSERT_FALSE(queue_pop(que, &n));

    /*  Test attributes of emptied queue  */

    TEST_ASSERT_EQUAL(queue_capacity(que), 3);
    TEST_ASSERT_EQUAL(queue_size(que), 0);
    TEST_ASSERT_EQUAL(queue_free_space(que), 3);
    TEST_ASSERT_TRUE(queue_is_empty(que));
    TEST_ASSERT_FALSE(queue_is_full(que));

    queue_destroy(que);
}

/*  Test that dynamically allocated strings are appropriately
 *  freed when a queue is destroyed. Run this test case through
 *  Valgrind or a similar tool to check no memory leaks.         */

TEST_CASE(test_queue_free_strings)
{
    Queue queue = queue_create(1, DATATYPE_STRING,
                               GDS_RESIZABLE | GDS_FREE_ON_DESTROY);
    if ( !queue ) {
        perror("couldn't create queue");
        exit(EXIT_FAILURE);
    }

    TEST_ASSERT_TRUE(queue_push(queue, strdup("First string")));
    TEST_ASSERT_TRUE(queue_push(queue, strdup("Second string")));
    TEST_ASSERT_TRUE(queue_push(queue, strdup("Third string")));
    TEST_ASSERT_TRUE(queue_push(queue, strdup("Fourth string")));

    /*  Test attributes of emptied queue  */

    TEST_ASSERT_EQUAL(queue_capacity(queue), 4);
    TEST_ASSERT_EQUAL(queue_size(queue), 4);
    TEST_ASSERT_EQUAL(queue_free_space(queue), 0);
    TEST_ASSERT_FALSE(queue_is_empty(queue));
    TEST_ASSERT_TRUE(queue_is_full(queue));
    
    /*  Destroy queue while containing dynamically allocated elements  */

    queue_destroy(queue);
}

void test_queue(void)
{
    RUN_CASE(test_queue_basic_ops);
    RUN_CASE(test_queue_free_strings);
}
