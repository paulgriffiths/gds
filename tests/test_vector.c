/*  Unit tests for generic vector data structure  */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pggds/vector.h>
#include <pggds/unittest.h>
#include "test_vector.h"
#include "test_struct.h"

TEST_SUITE(test_vector);

/*  Test basic functionality of vector with zero initial size  */

TEST_CASE(test_vector_zero)
{
    Vector vector = vector_create(0, DATATYPE_INT, 0);
    if ( !vector ) {
        perror("couldn't create vector");
        exit(EXIT_FAILURE);
    }

    int n;
        
    /*  Test vector attributes when empty  */

    TEST_ASSERT_EQUAL(vector_length(vector), 0);
    TEST_ASSERT_EQUAL(vector_capacity(vector), 0);
    TEST_ASSERT_EQUAL(vector_free_space(vector), 0);
    TEST_ASSERT_TRUE(vector_is_empty(vector));

    /*  Test appending some elements  */

    TEST_ASSERT_TRUE(vector_append(vector, 3));
    TEST_ASSERT_TRUE(vector_append(vector, 4));
    TEST_ASSERT_TRUE(vector_append(vector, 5));

    TEST_ASSERT_TRUE(vector_element_at_index(vector, 0, &n));
    TEST_ASSERT_EQUAL(n, 3);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 1, &n));
    TEST_ASSERT_EQUAL(n, 4);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 2, &n));
    TEST_ASSERT_EQUAL(n, 5);

    /*  Test out-of-index fails  */

    TEST_ASSERT_FALSE(vector_element_at_index(vector, 3, &n));

    vector_destroy(vector);
}

/*  Test creation and destruction of zero-size vector. Run this
 *  test case through Valgrind or a similar tool to check no
 *  memory issues.                                               */

TEST_CASE(test_vector_zero_destroy)
{
    Vector vector = vector_create(0, DATATYPE_INT, 0);
    if ( !vector ) {
        perror("couldn't create vector");
        exit(EXIT_FAILURE);
    }

    /*  Silence unused variable warning  */

    TEST_ASSERT_TRUE(1);

    vector_destroy(vector);
}

/*  Test basic functionality  */

TEST_CASE(test_vector_basic)
{
    Vector vector = vector_create(3, DATATYPE_INT, 0);
    if ( !vector ) {
        perror("couldn't create vector");
        exit(EXIT_FAILURE);
    }

    int n;
        
    /*  Test vector attributes when empty  */

    TEST_ASSERT_EQUAL(vector_length(vector), 3);
    TEST_ASSERT_EQUAL(vector_capacity(vector), 3);
    TEST_ASSERT_EQUAL(vector_free_space(vector), 0);
    TEST_ASSERT_FALSE(vector_is_empty(vector));

    /*  Test retrieval and delete fail for out-of-range  */

    TEST_ASSERT_FALSE(vector_element_at_index(vector, 4, &n));
    TEST_ASSERT_FALSE(vector_delete_index(vector, 4));

    /*  Test setting and retrieval of elements  */

    TEST_ASSERT_TRUE(vector_set_element_at_index(vector, 0, 4));
    TEST_ASSERT_TRUE(vector_set_element_at_index(vector, 1, 5));
    TEST_ASSERT_TRUE(vector_set_element_at_index(vector, 2, 6));

    TEST_ASSERT_TRUE(vector_element_at_index(vector, 0, &n));
    TEST_ASSERT_EQUAL(n, 4);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 1, &n));
    TEST_ASSERT_EQUAL(n, 5);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 2, &n));
    TEST_ASSERT_EQUAL(n, 6);

    /*  Test relevant vector attributes again  */

    TEST_ASSERT_EQUAL(vector_length(vector), 3);
    TEST_ASSERT_FALSE(vector_is_empty(vector));

    /*  Test prepending a new element  */

    TEST_ASSERT_TRUE(vector_prepend(vector, 3));

    TEST_ASSERT_TRUE(vector_element_at_index(vector, 0, &n));
    TEST_ASSERT_EQUAL(n, 3);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 1, &n));
    TEST_ASSERT_EQUAL(n, 4);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 2, &n));
    TEST_ASSERT_EQUAL(n, 5);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 3, &n));
    TEST_ASSERT_EQUAL(n, 6);

    /*  Test vector attributes after prepending  */

    TEST_ASSERT_EQUAL(vector_length(vector), 4);
    TEST_ASSERT_EQUAL(vector_capacity(vector), 6);
    TEST_ASSERT_EQUAL(vector_free_space(vector), 2);
    TEST_ASSERT_FALSE(vector_is_empty(vector));

    /*  Test insert element at middle  */

    TEST_ASSERT_TRUE(vector_insert(vector, 2, 7));

    TEST_ASSERT_TRUE(vector_element_at_index(vector, 0, &n));
    TEST_ASSERT_EQUAL(n, 3);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 1, &n));
    TEST_ASSERT_EQUAL(n, 4);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 2, &n));
    TEST_ASSERT_EQUAL(n, 7);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 3, &n));
    TEST_ASSERT_EQUAL(n, 5);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 4, &n));
    TEST_ASSERT_EQUAL(n, 6);

    /*  Test vector attributes after inserting  */

    TEST_ASSERT_EQUAL(vector_length(vector), 5);
    TEST_ASSERT_EQUAL(vector_capacity(vector), 6);
    TEST_ASSERT_EQUAL(vector_free_space(vector), 1);
    TEST_ASSERT_FALSE(vector_is_empty(vector));

    /*  Test appending a new element  */

    TEST_ASSERT_TRUE(vector_append(vector, 8));

    TEST_ASSERT_TRUE(vector_element_at_index(vector, 0, &n));
    TEST_ASSERT_EQUAL(n, 3);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 1, &n));
    TEST_ASSERT_EQUAL(n, 4);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 2, &n));
    TEST_ASSERT_EQUAL(n, 7);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 3, &n));
    TEST_ASSERT_EQUAL(n, 5);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 4, &n));
    TEST_ASSERT_EQUAL(n, 6);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 5, &n));
    TEST_ASSERT_EQUAL(n, 8);

    /*  Test vector attributes after inserting  */

    TEST_ASSERT_EQUAL(vector_length(vector), 6);
    TEST_ASSERT_EQUAL(vector_capacity(vector), 6);
    TEST_ASSERT_EQUAL(vector_free_space(vector), 0);
    TEST_ASSERT_FALSE(vector_is_empty(vector));

    /*  Test various deletions  */
    
    TEST_ASSERT_TRUE(vector_delete_front(vector));
    TEST_ASSERT_TRUE(vector_delete_back(vector));
    TEST_ASSERT_TRUE(vector_delete_index(vector, 2));

    TEST_ASSERT_TRUE(vector_element_at_index(vector, 0, &n));
    TEST_ASSERT_EQUAL(n, 4);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 1, &n));
    TEST_ASSERT_EQUAL(n, 7);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 2, &n));
    TEST_ASSERT_EQUAL(n, 6);

    /*  Test vector attributes after inserting  */

    TEST_ASSERT_EQUAL(vector_length(vector), 3);
    TEST_ASSERT_EQUAL(vector_capacity(vector), 6);
    TEST_ASSERT_EQUAL(vector_free_space(vector), 3);
    TEST_ASSERT_FALSE(vector_is_empty(vector));

    /*  Test setting values for existing elements  */
    
    TEST_ASSERT_TRUE(vector_set_element_at_index(vector, 0, 11));
    TEST_ASSERT_TRUE(vector_set_element_at_index(vector, 2, 9));

    TEST_ASSERT_TRUE(vector_element_at_index(vector, 0, &n));
    TEST_ASSERT_EQUAL(n, 11);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 1, &n));
    TEST_ASSERT_EQUAL(n, 7);
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 2, &n));
    TEST_ASSERT_EQUAL(n, 9);

    /*  Test vector attributes after inserting  */

    TEST_ASSERT_EQUAL(vector_length(vector), 3);
    TEST_ASSERT_EQUAL(vector_capacity(vector), 6);
    TEST_ASSERT_EQUAL(vector_free_space(vector), 3);
    TEST_ASSERT_FALSE(vector_is_empty(vector));

    vector_destroy(vector);
}

/*  Test that dynamically allocated strings are appropriately
 *  freed when a vector is destroyed. Run this test case through
 *  Valgrind or a similar tool to check no memory leaks.          */

TEST_CASE(test_vector_free_strings)
{
    Vector vector = vector_create(1, DATATYPE_STRING, GDS_FREE_ON_DESTROY);
    if ( !vector ) {
        perror("couldn't create vector");
        exit(EXIT_FAILURE);
    }

    char * pc;

    /*  Initialize initial element and check attributes  */

    TEST_ASSERT_TRUE(vector_set_element_at_index(vector, 0, (void *) NULL));
    TEST_ASSERT_EQUAL(vector_length(vector), 1);
    TEST_ASSERT_EQUAL(vector_capacity(vector), 1);

    /*  Append some elements and check attributes  */

    TEST_ASSERT_TRUE(vector_append(vector, strdup("First string")));
    TEST_ASSERT_EQUAL(vector_length(vector), 2);
    TEST_ASSERT_EQUAL(vector_capacity(vector), 2);
    TEST_ASSERT_EQUAL(vector_free_space(vector), 0);

    TEST_ASSERT_TRUE(vector_append(vector, strdup("Second string")));
    TEST_ASSERT_EQUAL(vector_length(vector), 3);
    TEST_ASSERT_EQUAL(vector_capacity(vector), 4);
    TEST_ASSERT_EQUAL(vector_free_space(vector), 1);

    TEST_ASSERT_TRUE(vector_append(vector, strdup("Third string")));
    TEST_ASSERT_EQUAL(vector_length(vector), 4);
    TEST_ASSERT_EQUAL(vector_capacity(vector), 4);
    TEST_ASSERT_EQUAL(vector_free_space(vector), 0);
    TEST_ASSERT_FALSE(vector_is_empty(vector));
    
    /*  Test values of elements  */
    
    TEST_ASSERT_TRUE(vector_element_at_index(vector, 0, &pc));
    TEST_ASSERT_EQUAL(pc, NULL);

    TEST_ASSERT_TRUE(vector_element_at_index(vector, 1, &pc));
    TEST_ASSERT_STR_EQUAL(pc, "First string");

    TEST_ASSERT_TRUE(vector_element_at_index(vector, 2, &pc));
    TEST_ASSERT_STR_EQUAL(pc, "Second string");

    TEST_ASSERT_TRUE(vector_element_at_index(vector, 3, &pc));
    TEST_ASSERT_STR_EQUAL(pc, "Third string");

    /*  Delete an element, check new value, and check attributes  */

    TEST_ASSERT_TRUE(vector_delete_index(vector, 2));

    TEST_ASSERT_TRUE(vector_element_at_index(vector, 2, &pc));
    TEST_ASSERT_STR_EQUAL(pc, "Third string");

    TEST_ASSERT_EQUAL(vector_length(vector), 3);
    TEST_ASSERT_EQUAL(vector_capacity(vector), 4);
    TEST_ASSERT_EQUAL(vector_free_space(vector), 1);
    TEST_ASSERT_FALSE(vector_is_empty(vector));
    
    vector_destroy(vector);
}

/*  Test find function  */

TEST_CASE(test_vector_find)
{
    Vector vector = vector_create(1, DATATYPE_UNSIGNED_LONG_LONG,
                                  GDS_RESIZABLE);
    if ( !vector ) {
        perror("couldn't create vector");
        exit(EXIT_FAILURE);
    }

    vector_set_element_at_index(vector, 0, 10ULL);
    vector_append(vector, 20ULL);
    vector_append(vector, 40ULL);
    vector_append(vector, 30ULL);

    size_t index;

    /*  Check values are found at correct indices  */

    TEST_ASSERT_TRUE(vector_find(vector, &index, 10ULL));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(vector_find(vector, &index, 20ULL));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(vector_find(vector, &index, 40ULL));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(vector_find(vector, &index, 30ULL));
    TEST_ASSERT_EQUAL(index, 3);

    /*  Test missing value is not found  */

    TEST_ASSERT_FALSE(vector_find(vector, &index, 50ULL));

    vector_destroy(vector);
}

/*  Test find function with struct elements  */

TEST_CASE(test_vector_find_struct)
{
    struct hms h1 = {1, 2, 3};
    struct hms h2 = {1, 2, 4};
    struct hms h3 = {2, 2, 3};
    struct hms h4 = {4, 1, 1};
    struct hms h5 = {5, 2, 4};

    Vector vector = vector_create(4, DATATYPE_POINTER, 0, compare_hms);
    if ( !vector ) {
        perror("couldn't create vector");
        exit(EXIT_FAILURE);
    }

    vector_set_element_at_index(vector, 0, (void *) &h1);
    vector_set_element_at_index(vector, 1, (void *) &h2);
    vector_set_element_at_index(vector, 2, (void *) &h3);
    vector_set_element_at_index(vector, 3, (void *) &h4);

    size_t index;

    /*  Test elements are found at correct indices  */

    TEST_ASSERT_TRUE(vector_find(vector, &index, (void *) &h1));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (void *) &h2));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (void *) &h3));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (void *) &h4));
    TEST_ASSERT_EQUAL(index, 3);

    /*  Test absent element is not found  */

    TEST_ASSERT_FALSE(vector_find(vector, &index, (void *) &h5));

    vector_destroy(vector);
}

/*  Test sort function with strings  */

TEST_CASE(test_vector_sort_strings)
{
    Vector vector = vector_create(4, DATATYPE_STRING, GDS_FREE_ON_DESTROY);
    if ( !vector ) {
        perror("couldn't create vector");
        exit(EXIT_FAILURE);
    }

    vector_set_element_at_index(vector, 0, strdup("Elephant"));
    vector_set_element_at_index(vector, 1, strdup("Dog"));
    vector_set_element_at_index(vector, 2, strdup("Giraffe"));
    vector_set_element_at_index(vector, 3, strdup("Aardvark"));

    size_t index;

    /*  Check elements are found at expected indices prior to sorting  */

    TEST_ASSERT_TRUE(vector_find(vector, &index, "Elephant"));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(vector_find(vector, &index, "Dog"));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(vector_find(vector, &index, "Giraffe"));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(vector_find(vector, &index, "Aardvark"));
    TEST_ASSERT_EQUAL(index, 3);

    /*  Check absent element is not found  */

    TEST_ASSERT_FALSE(vector_find(vector, &index, "Pelican"));

    /*  Check elements are found at expected indices after sorting  */

    vector_sort(vector);

    TEST_ASSERT_TRUE(vector_find(vector, &index, "Aardvark"));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(vector_find(vector, &index, "Dog"));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(vector_find(vector, &index, "Elephant"));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(vector_find(vector, &index, "Giraffe"));
    TEST_ASSERT_EQUAL(index, 3);

    vector_destroy(vector);
}

/*  Test sort function with scalar elements  */

TEST_CASE(test_vector_sort_sizet)
{
    Vector vector = vector_create(4, DATATYPE_SIZE_T, 0);
    if ( !vector ) {
        perror("couldn't create vector");
        exit(EXIT_FAILURE);
    }

    vector_set_element_at_index(vector, 0, (size_t) 100);
    vector_set_element_at_index(vector, 1, (size_t) 400);
    vector_set_element_at_index(vector, 2, (size_t) 200);
    vector_set_element_at_index(vector, 3, (size_t) 300);

    size_t index;

    /*  Check elements are found at expected indices prior to sorting  */

    TEST_ASSERT_TRUE(vector_find(vector, &index, (size_t) 100));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (size_t) 400));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (size_t) 200));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (size_t) 300));
    TEST_ASSERT_EQUAL(index, 3);

    /*  Check absent value is not found  */

    TEST_ASSERT_FALSE(vector_find(vector, &index, (size_t) 500));

    /*  Check elements are found at expected indices after sorting  */

    vector_sort(vector);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (size_t) 100));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (size_t) 200));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (size_t) 300));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (size_t) 400));
    TEST_ASSERT_EQUAL(index, 3);

    vector_destroy(vector);
}

/*  Test sort function with struct elements  */

TEST_CASE(test_vector_sort_struct)
{
    struct hms h1 = {1, 2, 3};
    struct hms h2 = {1, 2, 4};
    struct hms h3 = {2, 2, 3};
    struct hms h4 = {4, 1, 1};
    struct hms h5 = {5, 2, 4};

    Vector vector = vector_create(4, DATATYPE_POINTER, 0, compare_hms);
    if ( !vector ) {
        perror("couldn't create vector");
        exit(EXIT_FAILURE);
    }

    vector_set_element_at_index(vector, 0, (void *) &h3);
    vector_set_element_at_index(vector, 1, (void *) &h2);
    vector_set_element_at_index(vector, 2, (void *) &h1);
    vector_set_element_at_index(vector, 3, (void *) &h4);

    size_t index;

    /*  Check elements are found at expected indices prior to sorting  */

    TEST_ASSERT_TRUE(vector_find(vector, &index, (void *) &h3));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (void *) &h2));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (void *) &h1));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (void *) &h4));
    TEST_ASSERT_EQUAL(index, 3);

    /*  Check absent element is not found  */

    TEST_ASSERT_FALSE(vector_find(vector, &index, (void *) &h5));

    /*  Check elements are found at expected indices prior to sorting  */

    vector_sort(vector);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (void *) &h1));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (void *) &h2));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (void *) &h3));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(vector_find(vector, &index, (void *) &h4));
    TEST_ASSERT_EQUAL(index, 3);

    vector_destroy(vector);
}

/*  Tests reverse sort function  */

TEST_CASE(test_vector_reverse_sort)
{
    Vector vector = vector_create(4, DATATYPE_LONG, 0);
    if ( !vector ) {
        perror("couldn't create vector");
        exit(EXIT_FAILURE);
    }

    vector_set_element_at_index(vector, 0, 100L);
    vector_set_element_at_index(vector, 1, 400L);
    vector_set_element_at_index(vector, 2, 200L);
    vector_set_element_at_index(vector, 3, 300L);

    size_t index;

    /*  Check elements are found at expected indices prior to sorting  */

    TEST_ASSERT_TRUE(vector_find(vector, &index, 100L));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(vector_find(vector, &index, 400L));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(vector_find(vector, &index, 200L));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(vector_find(vector, &index, 300L));
    TEST_ASSERT_EQUAL(index, 3);

    /*  Check elements are found at expected indices after sorting  */

    vector_reverse_sort(vector);

    TEST_ASSERT_TRUE(vector_find(vector, &index, 400L));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(vector_find(vector, &index, 300L));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(vector_find(vector, &index, 200L));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(vector_find(vector, &index, 100L));
    TEST_ASSERT_EQUAL(index, 3);

    vector_destroy(vector);
}

void test_vector(void)
{
    RUN_CASE(test_vector_zero);
    RUN_CASE(test_vector_zero_destroy);
    RUN_CASE(test_vector_basic);
    RUN_CASE(test_vector_free_strings);
    RUN_CASE(test_vector_find);
    RUN_CASE(test_vector_find_struct);
    RUN_CASE(test_vector_sort_strings);
    RUN_CASE(test_vector_sort_sizet);
    RUN_CASE(test_vector_sort_struct);
    RUN_CASE(test_vector_reverse_sort);
}
