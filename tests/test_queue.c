#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "test_queue.h"
#include "test_logging.h"

static void test_queue_one(void)
{
    Queue que = queue_create(3, DATATYPE_INT, 0);

    int n;
    size_t sz;
    bool status;
        
    sz = queue_capacity(que);
    tests_log_test(sz == 3, "queue_capacity() gave wrong value");

    sz = queue_size(que);
    tests_log_test(sz == 0, "queue_size() gave wrong value");

    sz = queue_free_space(que);
    tests_log_test(sz == 3, "queue_free_space() gave wrong value");

    status = queue_is_empty(que);
    tests_log_test(status, "queue_is_empty() gave wrong value");

    status = queue_is_full(que);
    tests_log_test(!status, "queue_is_full() gave wrong value");

    status = queue_pop(que, &n);
    tests_log_test(!status, "queue_pop() improperly succeeded");

    status = queue_push(que, 1);
    tests_log_test(status, "queue_push() failed");
    
    status = queue_push(que, -2);
    tests_log_test(status, "queue_push() failed");

    status = queue_push(que, 5);
    tests_log_test(status, "queue_push() failed");

    status = queue_push(que, 7);
    tests_log_test(!status, "queue_push() improperly succeeded");

    status = queue_peek(que, &n);
    tests_log_test(status, "queue_peek() failed");
    tests_log_test(n == 1, "queue_peek() gave wrong value");

    sz = queue_capacity(que);
    tests_log_test(sz == 3, "queue_capacity() gave wrong value");

    sz = queue_size(que);
    tests_log_test(sz == 3, "queue_size() gave wrong value");

    sz = queue_free_space(que);
    tests_log_test(sz == 0, "queue_free_space() gave wrong value");

    status = queue_is_empty(que);
    tests_log_test(!status, "queue_is_empty() gave wrong value");

    status = queue_is_full(que);
    tests_log_test(status, "queue_is_full() gave wrong value");

    status = queue_pop(que, &n);
    tests_log_test(status, "queue_pop() failed");
    tests_log_test(n == 1, "queue_pop() gave wrong value");

    status = queue_pop(que, &n);
    tests_log_test(status, "queue_pop() failed");
    tests_log_test(n == -2, "queue_pop() gave wrong value");

    status = queue_peek(que, &n);
    tests_log_test(status, "queue_peek() failed");
    tests_log_test(n == 5, "queue_peek() gave wrong value");

    sz = queue_capacity(que);
    tests_log_test(sz == 3, "queue_capacity() gave wrong value");

    sz = queue_size(que);
    tests_log_test(sz == 1, "queue_size() gave wrong value");

    sz = queue_free_space(que);
    tests_log_test(sz == 2, "queue_free_space() gave wrong value");

    status = queue_is_empty(que);
    tests_log_test(!status, "queue_is_empty() gave wrong value");

    status = queue_is_full(que);
    tests_log_test(!status, "queue_is_full() gave wrong value");

    status = queue_pop(que, &n);
    tests_log_test(status, "queue_pop() failed");
    tests_log_test(n == 5, "queue_pop() gave wrong value");

    sz = queue_capacity(que);
    tests_log_test(sz == 3, "queue_capacity() gave wrong value");

    sz = queue_size(que);
    tests_log_test(sz == 0, "queue_size() gave wrong value");

    sz = queue_free_space(que);
    tests_log_test(sz == 3, "queue_free_space() gave wrong value");

    status = queue_is_empty(que);
    tests_log_test(status, "queue_is_empty() gave wrong value");

    status = queue_is_full(que);
    tests_log_test(!status, "queue_is_full() gave wrong value");

    queue_destroy(que);
}

static void test_queue_free_strings(void)
{
    Queue queue = queue_create(1, DATATYPE_STRING,
                               GDS_RESIZABLE | GDS_FREE_ON_DESTROY);
    if ( !queue ) {
        perror("couldn't create queue");
        exit(EXIT_FAILURE);
    }

    bool status;
    size_t sz;

    status = queue_push(queue, strdup("First string"));
    tests_log_test(status, "queue_push() failed");

    status = queue_push(queue, strdup("Second string"));
    tests_log_test(status, "queue_push() failed");

    status = queue_push(queue, strdup("Third string"));
    tests_log_test(status, "queue_push() failed");

    status = queue_push(queue, strdup("Fourth string"));
    tests_log_test(status, "queue_push() failed");

    sz = queue_size(queue);
    tests_log_test(sz == 4, "queue_length() gave wrong value");

    sz = queue_capacity(queue);
    tests_log_test(sz == 4, "queue_capacity() gave wrong value");

    sz = queue_free_space(queue);
    tests_log_test(sz == 0, "queue_free_space() gave wrong value");

    status = queue_is_empty(queue);
    tests_log_test(!status, "queue_is_empty() gave wrong value");
    
    queue_destroy(queue);
}

void test_queue(void)
{
    test_queue_one();
    test_queue_free_strings();
}
