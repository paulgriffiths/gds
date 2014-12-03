/*  Unit tests for generic list data structure  */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pggds/list.h>
#include <pggds/unittest.h>
#include <pggds/string_util.h>
#include "test_list.h"
#include "test_struct.h"

TEST_SUITE(test_list);

/*  Test basic operations  */

TEST_CASE(test_list_basic)
{
    List list = list_create(DATATYPE_INT, 0);
    if ( !list ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    int n;
        
    /*  Test list attributes when empty  */

    TEST_ASSERT_EQUAL(list_length(list), 0);
    TEST_ASSERT_TRUE(list_is_empty(list));

    /*  Test out-of-range operations fail  */

    TEST_ASSERT_FALSE(list_element_at_index(list, 0, &n));
    TEST_ASSERT_FALSE(list_delete_index(list, 0));

    /*  Test insertion at front  */

    TEST_ASSERT_TRUE(list_insert(list, 0, 4));
    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &n));
    TEST_ASSERT_EQUAL(n, 4);

    TEST_ASSERT_EQUAL(list_length(list), 1);
    TEST_ASSERT_FALSE(list_is_empty(list));
    
    TEST_ASSERT_TRUE(list_insert(list, 0, 5));
    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &n));
    TEST_ASSERT_EQUAL(n, 5);

    TEST_ASSERT_TRUE(list_element_at_index(list, 1, &n));
    TEST_ASSERT_EQUAL(n, 4);

    TEST_ASSERT_EQUAL(list_length(list), 2);
    TEST_ASSERT_FALSE(list_is_empty(list));
    
    /*  Test insertion at end  */

    TEST_ASSERT_TRUE(list_insert(list, 2, 7));
    TEST_ASSERT_TRUE(list_element_at_index(list, 2, &n));
    TEST_ASSERT_EQUAL(n, 7);

    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &n));
    TEST_ASSERT_EQUAL(n, 5);

    TEST_ASSERT_TRUE(list_element_at_index(list, 1, &n));
    TEST_ASSERT_EQUAL(n, 4);

    TEST_ASSERT_EQUAL(list_length(list), 3);
    TEST_ASSERT_FALSE(list_is_empty(list));
    
    /*  Test index out-of-range fails  */

    TEST_ASSERT_FALSE(list_element_at_index(list, 3, &n));
    TEST_ASSERT_FALSE(list_delete_index(list, 3));

    /*  Test deleting from front  */

    TEST_ASSERT_TRUE(list_delete_front(list));

    TEST_ASSERT_EQUAL(list_length(list), 2);
    TEST_ASSERT_FALSE(list_is_empty(list));
    
    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &n));
    TEST_ASSERT_EQUAL(n, 4);

    TEST_ASSERT_TRUE(list_element_at_index(list, 1, &n));
    TEST_ASSERT_EQUAL(n, 7);

    /*  Test deleting from back  */

    TEST_ASSERT_TRUE(list_delete_back(list));

    TEST_ASSERT_EQUAL(list_length(list), 1);
    TEST_ASSERT_FALSE(list_is_empty(list));
    
    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &n));
    TEST_ASSERT_EQUAL(n, 4);

    /*  Test deleting last element from back  */

    TEST_ASSERT_TRUE(list_delete_back(list));

    TEST_ASSERT_EQUAL(list_length(list), 0);
    TEST_ASSERT_TRUE(list_is_empty(list));
    
    /*  Test appending and prepending items  */

    TEST_ASSERT_TRUE(list_append(list, 11));
    TEST_ASSERT_TRUE(list_append(list, 12));
    TEST_ASSERT_TRUE(list_prepend(list, 10));
    TEST_ASSERT_TRUE(list_prepend(list, 9));

    TEST_ASSERT_EQUAL(list_length(list), 4);
    TEST_ASSERT_FALSE(list_is_empty(list));

    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &n));
    TEST_ASSERT_EQUAL(n, 9);
    TEST_ASSERT_TRUE(list_element_at_index(list, 1, &n));
    TEST_ASSERT_EQUAL(n, 10);
    TEST_ASSERT_TRUE(list_element_at_index(list, 2, &n));
    TEST_ASSERT_EQUAL(n, 11);
    TEST_ASSERT_TRUE(list_element_at_index(list, 3, &n));
    TEST_ASSERT_EQUAL(n, 12);

    /*  Test out-of-range access fails  */

    TEST_ASSERT_FALSE(list_element_at_index(list, 4, &n));

    /*  Iteratively delete each item  */

    while ( !list_is_empty(list) ) {
        TEST_ASSERT_TRUE(list_delete_front(list));
    }

    TEST_ASSERT_EQUAL(list_length(list), 0);
    TEST_ASSERT_TRUE(list_is_empty(list));
    
    list_destroy(list);
}

/*  Test that dynamically allocated strings are appropriately
 *  freed when a list is destroyed. Run this test case through
 *  Valgrind or a similar tool to check no memory leaks.        */

TEST_CASE(test_list_free_strings)
{
    List list = list_create(DATATYPE_STRING, GDS_FREE_ON_DESTROY);
    if ( !list ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    char * pc;

    /*  Append some dynamically allocated strings  */

    TEST_ASSERT_TRUE(list_append(list, gds_strdup("First string")));
    TEST_ASSERT_TRUE(list_append(list, gds_strdup("Second string")));
    TEST_ASSERT_TRUE(list_append(list, gds_strdup("Third string")));

    TEST_ASSERT_EQUAL(list_length(list), 3);
    TEST_ASSERT_FALSE(list_is_empty(list));
    
    /*  Check values are as expected  */

    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &pc));
    TEST_ASSERT_STR_EQUAL(pc, "First string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 1, &pc));
    TEST_ASSERT_STR_EQUAL(pc, "Second string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 2, &pc));
    TEST_ASSERT_STR_EQUAL(pc, "Third string");

    /*  Destroy list with dynamically allocated elements still present  */

    list_destroy(list);
}

/*  Test setting of specified element by index  */

TEST_CASE(test_list_set_element)
{
    List list = list_create(DATATYPE_UNSIGNED_LONG, 0);
    if ( !list ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    unsigned long l;

    /*  Append some values  */

    TEST_ASSERT_TRUE(list_append(list, 42UL));
    TEST_ASSERT_TRUE(list_append(list, 53UL));
    TEST_ASSERT_TRUE(list_append(list, 65UL));
    TEST_ASSERT_TRUE(list_append(list, 76UL));

    /*  Check values are as expected  */

    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &l));
    TEST_ASSERT_EQUAL(l, 42L);

    TEST_ASSERT_TRUE(list_element_at_index(list, 1, &l));
    TEST_ASSERT_EQUAL(l, 53L);

    TEST_ASSERT_TRUE(list_element_at_index(list, 2, &l));
    TEST_ASSERT_EQUAL(l, 65L);

    TEST_ASSERT_TRUE(list_element_at_index(list, 3, &l));
    TEST_ASSERT_EQUAL(l, 76L);

    /*  Set a couple of values  */

    TEST_ASSERT_TRUE(list_set_element_at_index(list, 1, 87UL));
    TEST_ASSERT_TRUE(list_set_element_at_index(list, 2, 98UL));

    /*  Check values are as expected after setting  */

    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &l));
    TEST_ASSERT_EQUAL(l, 42L);

    TEST_ASSERT_TRUE(list_element_at_index(list, 1, &l));
    TEST_ASSERT_EQUAL(l, 87L);

    TEST_ASSERT_TRUE(list_element_at_index(list, 2, &l));
    TEST_ASSERT_EQUAL(l, 98L);

    TEST_ASSERT_TRUE(list_element_at_index(list, 3, &l));
    TEST_ASSERT_EQUAL(l, 76L);

    list_destroy(list);
}

/*  Test find function  */

TEST_CASE(test_list_find)
{
    List list = list_create(DATATYPE_SIGNED_CHAR, 0);
    if ( !list ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    /*  Append some elements  */

    list_append(list, 'c');
    list_append(list, 'd');
    list_append(list, 'f');
    list_append(list, 'e');

    size_t index;

    /*  Check they're found at the expected indices  */

    TEST_ASSERT_TRUE(list_find(list, &index, 'c'));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(list_find(list, &index, 'd'));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(list_find(list, &index, 'f'));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(list_find(list, &index, 'e'));
    TEST_ASSERT_EQUAL(index, 3);

    /*  Check absent value is not found  */

    TEST_ASSERT_FALSE(list_find(list, &index, 'g'));

    list_destroy(list);
}

/*  Test find function with struct members  */

TEST_CASE(test_list_find_struct)
{
    struct hms h1 = {1, 2, 3};
    struct hms h2 = {1, 2, 4};
    struct hms h3 = {2, 2, 3};
    struct hms h4 = {4, 1, 1};
    struct hms h5 = {5, 2, 4};

    List list = list_create(DATATYPE_POINTER, 0, compare_hms);
    if ( !list ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    /*  Append some elements  */

    list_append(list, (void *) &h1);
    list_append(list, (void *) &h2);
    list_append(list, (void *) &h3);
    list_append(list, (void *) &h4);

    size_t index;

    /*  Check they're found at the expected indices  */

    TEST_ASSERT_TRUE(list_find(list, &index, (void *) &h1));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(list_find(list, &index, (void *) &h2));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(list_find(list, &index, (void *) &h3));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(list_find(list, &index, (void *) &h4));
    TEST_ASSERT_EQUAL(index, 3);

    /*  Test absent element is not found  */

    TEST_ASSERT_FALSE(list_find(list, &index, (void *) &h5));

    list_destroy(list);
}

/*  Test sort function with scalar elements  */

TEST_CASE(test_list_sort_sizet)
{
    List list = list_create(DATATYPE_SIZE_T, 0);
    if ( !list ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    /*  Append some values  */

    list_append(list, (size_t) 100);
    list_append(list, (size_t) 400);
    list_append(list, (size_t) 200);
    list_append(list, (size_t) 300);

    size_t index;

    /*  Check they're at the expected indices prior to sorting  */

    TEST_ASSERT_TRUE(list_find(list, &index, (size_t) 100));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(list_find(list, &index, (size_t) 400));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(list_find(list, &index, (size_t) 200));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(list_find(list, &index, (size_t) 300));
    TEST_ASSERT_EQUAL(index, 3);

    /*  Check absent element is not found  */

    TEST_ASSERT_FALSE(list_find(list, &index, (size_t) 500));

    /*  Check they're at the expected indices after sorting  */

    list_sort(list);

    TEST_ASSERT_TRUE(list_find(list, &index, (size_t) 100));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(list_find(list, &index, (size_t) 200));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(list_find(list, &index, (size_t) 300));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(list_find(list, &index, (size_t) 400));
    TEST_ASSERT_EQUAL(index, 3);

    list_destroy(list);
}

/*  Test sort function with strings  */

TEST_CASE(test_list_sort_strings)
{
    List list = list_create(DATATYPE_STRING, GDS_FREE_ON_DESTROY);
    if ( !list ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    /*  Append some elements  */

    list_append(list, gds_strdup("Elephant"));
    list_append(list, gds_strdup("Dog"));
    list_append(list, gds_strdup("Giraffe"));
    list_append(list, gds_strdup("Aardvark"));

    size_t index;

    /*  Check they're at the expected indices prior to sorting  */

    TEST_ASSERT_TRUE(list_find(list, &index, "Elephant"));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(list_find(list, &index, "Dog"));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(list_find(list, &index, "Giraffe"));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(list_find(list, &index, "Aardvark"));
    TEST_ASSERT_EQUAL(index, 3);

    /*  Check absent value is not found  */

    TEST_ASSERT_FALSE(list_find(list, &index, "Pelican"));

    /*  Check they're at the expect indices after sorting  */

    list_sort(list);

    TEST_ASSERT_TRUE(list_find(list, &index, "Aardvark"));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(list_find(list, &index, "Dog"));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(list_find(list, &index, "Elephant"));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(list_find(list, &index, "Giraffe"));
    TEST_ASSERT_EQUAL(index, 3);

    list_destroy(list);
}

/*  Test sort function with struct members  */

TEST_CASE(test_list_sort_struct)
{
    struct hms h1 = {1, 2, 3};
    struct hms h2 = {1, 2, 4};
    struct hms h3 = {2, 2, 3};
    struct hms h4 = {4, 1, 1};
    struct hms h5 = {5, 2, 4};

    List list = list_create(DATATYPE_POINTER, 0, compare_hms);
    if ( !list ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    /*  Append some elements  */

    list_append(list, (void *) &h3);
    list_append(list, (void *) &h2);
    list_append(list, (void *) &h1);
    list_append(list, (void *) &h4);

    size_t index;

    /*  Check they're at the expected indices prior to sorting  */

    TEST_ASSERT_TRUE(list_find(list, &index, (void *) &h3));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(list_find(list, &index, (void *) &h2));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(list_find(list, &index, (void *) &h1));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(list_find(list, &index, (void *) &h4));
    TEST_ASSERT_EQUAL(index, 3);

    /*  Check absent value is not found  */

    TEST_ASSERT_FALSE(list_find(list, &index, (void *) &h5));

    /*  Check they're at the expected indices after sorting  */

    list_sort(list);

    TEST_ASSERT_TRUE(list_find(list, &index, (void *) &h1));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(list_find(list, &index, (void *) &h2));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(list_find(list, &index, (void *) &h3));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(list_find(list, &index, (void *) &h4));
    TEST_ASSERT_EQUAL(index, 3);

    list_destroy(list);
}

/*  Test reverse sort function  */

TEST_CASE(test_list_reverse_sort)
{
    List list = list_create(DATATYPE_LONG, 0);
    if ( !list ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    /*  Append some values  */

    list_append(list, 100L);
    list_append(list, 400L);
    list_append(list, 200L);
    list_append(list, 300L);

    size_t index;

    /*  Check they're at the expected indices prior to sorting  */

    TEST_ASSERT_TRUE(list_find(list, &index, 100L));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(list_find(list, &index, 400L));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(list_find(list, &index, 200L));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(list_find(list, &index, 300L));
    TEST_ASSERT_EQUAL(index, 3);

    /*  Check absent element is not found  */

    TEST_ASSERT_FALSE(list_find(list, &index, 500L));

    /*  Check they're at the expected indices after sorting  */

    list_reverse_sort(list);

    TEST_ASSERT_TRUE(list_find(list, &index, 400L));
    TEST_ASSERT_EQUAL(index, 0);

    TEST_ASSERT_TRUE(list_find(list, &index, 300L));
    TEST_ASSERT_EQUAL(index, 1);

    TEST_ASSERT_TRUE(list_find(list, &index, 200L));
    TEST_ASSERT_EQUAL(index, 2);

    TEST_ASSERT_TRUE(list_find(list, &index, 100L));
    TEST_ASSERT_EQUAL(index, 3);

    list_destroy(list);
}

/*  Test list traversal with forward iterator  */

TEST_CASE(test_list_itr)
{
    List list = list_create(DATATYPE_INT, 0);
    if ( !list ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    int some_ints[6] = {100, 400, 200, 300, 50, 22};

    /*  Append some values  */

    for ( size_t i = 0; i < 6; ++i ) {
        list_append(list, some_ints[i]);
    }

    /*  Iterate through and check them  */

    ListItr itr = list_itr_first(list);
    size_t j = 0;
    while ( itr ) {
        int n;
        list_get_value_itr(itr, &n);
        TEST_ASSERT_EQUAL(n, some_ints[j++]);
        itr = list_itr_next(itr);
    }

    list_destroy(list);
}

/*  Test list traversal with reverse iterator  */

TEST_CASE(test_list_itr_reverse)
{
    List list = list_create(DATATYPE_INT, 0);
    if ( !list ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }

    int some_ints[6] = {100, 400, 200, 300, 50, 22};

    /*  Append some values  */

    for ( size_t i = 0; i < 6; ++i ) {
        list_prepend(list, some_ints[i]);
    }

    /*  Iterate through in reverse and check them  */

    ListItr itr = list_itr_last(list);
    size_t j = 0;
    while ( itr ) {
        int n;
        list_get_value_itr(itr, &n);
        TEST_ASSERT_EQUAL(n, some_ints[j++]);
        itr = list_itr_previous(itr);
    }

    list_destroy(list);
}

/*  Test find with iterator function  */

TEST_CASE(test_list_itr_find)
{
    List list = list_create(DATATYPE_INT, 0);
    if ( !list ) {
        perror("couldn't create list");
        exit(EXIT_FAILURE);
    }
    
    /*  Append some values  */

    list_append(list, 10);
    list_append(list, 11);
    list_append(list, 12);
    list_append(list, 13);

    int n;

    ListItr itr;

    /*  Check correct iterator is returned for present values  */

    TEST_ASSERT_TRUE(itr = list_find_itr(list, 10));
    list_get_value_itr(itr, &n);
    TEST_ASSERT_EQUAL(n, 10);

    TEST_ASSERT_TRUE(itr = list_find_itr(list, 11));
    list_get_value_itr(itr, &n);
    TEST_ASSERT_EQUAL(n, 11);

    TEST_ASSERT_TRUE(itr = list_find_itr(list, 12));
    list_get_value_itr(itr, &n);
    TEST_ASSERT_EQUAL(n, 12);

    TEST_ASSERT_TRUE(itr = list_find_itr(list, 13));
    list_get_value_itr(itr, &n);
    TEST_ASSERT_EQUAL(n, 13);

    /*  Check NULL iterator is returned for absent values  */

    TEST_ASSERT_FALSE(itr = list_find_itr(list, 9));
    TEST_ASSERT_FALSE(itr = list_find_itr(list, 14));

    list_destroy(list);
}

/*  Test deletion via iterator  */

TEST_CASE(test_list_delete_itr)
{
    List list = list_create(DATATYPE_STRING, GDS_FREE_ON_DESTROY);
    list_append(list, gds_strdup("first string"));
    list_append(list, gds_strdup("second string"));
    list_append(list, gds_strdup("third string"));
    list_append(list, gds_strdup("fourth string"));
    list_append(list, gds_strdup("fifth string"));

    TEST_ASSERT_EQUAL(list_length(list), 5);
    
    char * c;

    /*  Get first itr, test, delete, and check list  */

    ListItr itr = list_itr_first(list);
    list_get_value_itr(itr, &c);
    TEST_ASSERT_STR_EQUAL(c, "first string");

    list_delete_itr(itr);
    TEST_ASSERT_EQUAL(list_length(list), 4);

    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &c));
    TEST_ASSERT_STR_EQUAL(c, "second string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 1, &c));
    TEST_ASSERT_STR_EQUAL(c, "third string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 2, &c));
    TEST_ASSERT_STR_EQUAL(c, "fourth string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 3, &c));
    TEST_ASSERT_STR_EQUAL(c, "fifth string");

    /*  Get last itr, test, delete, and check list  */

    itr = list_itr_last(list);
    list_get_value_itr(itr, &c);
    TEST_ASSERT_STR_EQUAL(c, "fifth string");

    list_delete_itr(itr);
    TEST_ASSERT_EQUAL(list_length(list), 3);

    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &c));
    TEST_ASSERT_STR_EQUAL(c, "second string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 1, &c));
    TEST_ASSERT_STR_EQUAL(c, "third string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 2, &c));
    TEST_ASSERT_STR_EQUAL(c, "fourth string");

    /*  Get middle itr, test, delete, and check list  */

    itr = list_itr_last(list);
    itr = list_itr_previous(itr);
    list_get_value_itr(itr, &c);
    TEST_ASSERT_STR_EQUAL(c, "third string");

    list_delete_itr(itr);
    TEST_ASSERT_EQUAL(list_length(list), 2);

    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &c));
    TEST_ASSERT_STR_EQUAL(c, "second string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 1, &c));
    TEST_ASSERT_STR_EQUAL(c, "fourth string");

    /*  Check no more elements  */

    TEST_ASSERT_FALSE(list_element_at_index(list, 2, &c));

    list_destroy(list);

    /*  Test iterative delete  */

    list = list_create(DATATYPE_INT, 0);
    list_append(list, 1);
    list_append(list, 2);
    list_append(list, 3);
    list_append(list, 4);
    list_append(list, 5);

    TEST_ASSERT_EQUAL(list_length(list), 5);
    TEST_ASSERT_FALSE(list_is_empty(list));

    itr = list_itr_first(list);
    while ( (itr = list_delete_itr(itr) ) ) {}

    TEST_ASSERT_EQUAL(list_length(list), 0);
    TEST_ASSERT_TRUE(list_is_empty(list));

    list_destroy(list);
}

/*  Test insertion before iterator  */

TEST_CASE(test_list_insert_before_itr)
{
    List list = list_create(DATATYPE_STRING, GDS_FREE_ON_DESTROY);

    list_append(list, gds_strdup("first string"));
    list_append(list, gds_strdup("second string"));
    list_append(list, gds_strdup("third string"));

    ListItr itr = list_itr_first(list);
    TEST_ASSERT_TRUE(list_insert_before_itr(itr, gds_strdup("fourth string")));
    itr = list_itr_next(itr);
    TEST_ASSERT_TRUE(list_insert_before_itr(itr, gds_strdup("fifth string")));
    itr = list_itr_next(itr);
    TEST_ASSERT_TRUE(list_insert_before_itr(itr, gds_strdup("sixth string")));

    TEST_ASSERT_EQUAL(list_length(list), 6);

    char * c;

    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &c));
    TEST_ASSERT_STR_EQUAL(c, "fourth string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 1, &c));
    TEST_ASSERT_STR_EQUAL(c, "first string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 2, &c));
    TEST_ASSERT_STR_EQUAL(c, "fifth string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 3, &c));
    TEST_ASSERT_STR_EQUAL(c, "second string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 4, &c));
    TEST_ASSERT_STR_EQUAL(c, "sixth string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 5, &c));
    TEST_ASSERT_STR_EQUAL(c, "third string");
    TEST_ASSERT_FALSE(list_element_at_index(list, 6, &c));

    list_destroy(list);
}

/*  Test insertion after iterator  */

TEST_CASE(test_list_insert_after_itr)
{
    List list = list_create(DATATYPE_STRING, GDS_FREE_ON_DESTROY);

    list_append(list, gds_strdup("first string"));
    list_append(list, gds_strdup("second string"));
    list_append(list, gds_strdup("third string"));

    ListItr itr = list_itr_first(list);
    TEST_ASSERT_TRUE(list_insert_after_itr(itr, gds_strdup("fourth string")));
    itr = list_itr_next(itr);
    itr = list_itr_next(itr);
    TEST_ASSERT_TRUE(list_insert_after_itr(itr, gds_strdup("fifth string")));
    itr = list_itr_next(itr);
    itr = list_itr_next(itr);
    TEST_ASSERT_TRUE(list_insert_after_itr(itr, gds_strdup("sixth string")));

    TEST_ASSERT_EQUAL(list_length(list), 6);

    char * c;

    TEST_ASSERT_TRUE(list_element_at_index(list, 0, &c));
    TEST_ASSERT_STR_EQUAL(c, "first string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 1, &c));
    TEST_ASSERT_STR_EQUAL(c, "fourth string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 2, &c));
    TEST_ASSERT_STR_EQUAL(c, "second string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 3, &c));
    TEST_ASSERT_STR_EQUAL(c, "fifth string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 4, &c));
    TEST_ASSERT_STR_EQUAL(c, "third string");
    TEST_ASSERT_TRUE(list_element_at_index(list, 5, &c));
    TEST_ASSERT_STR_EQUAL(c, "sixth string");
    TEST_ASSERT_FALSE(list_element_at_index(list, 6, &c));

    list_destroy(list);
}

void test_list(void)
{
    RUN_CASE(test_list_basic);
    RUN_CASE(test_list_free_strings);
    RUN_CASE(test_list_set_element);
    RUN_CASE(test_list_find);
    RUN_CASE(test_list_find_struct);
    RUN_CASE(test_list_sort_sizet);
    RUN_CASE(test_list_sort_strings);
    RUN_CASE(test_list_sort_struct);
    RUN_CASE(test_list_reverse_sort);
    RUN_CASE(test_list_itr);
    RUN_CASE(test_list_itr_reverse);
    RUN_CASE(test_list_itr_find);
    RUN_CASE(test_list_delete_itr);
    RUN_CASE(test_list_insert_before_itr);
    RUN_CASE(test_list_insert_after_itr);
}
