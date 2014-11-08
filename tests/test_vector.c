#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "test_vector.h"
#include "test_logging.h"

static void test_vector_basic(void)
{
    Vector vector = vector_create(3, DATATYPE_INT, 0);
    if ( !vector ) {
        perror("couldn't create vector");
        exit(EXIT_FAILURE);
    }

    int n;
    size_t sz;
    bool status;
        
    sz = vector_length(vector);
    tests_log_test(sz == 3, "vector_length() gave wrong value");

    sz = vector_capacity(vector);
    tests_log_test(sz == 3, "vector_capacity() gave wrong value");

    sz = vector_free_space(vector);
    tests_log_test(sz == 0, "vector_free_space() gave wrong value");

    status = vector_is_empty(vector);
    tests_log_test(!status, "vector_is_empty() gave wrong value");
    
    status = vector_element_at_index(vector, 4, &n);
    tests_log_test(!status, "vector_element_at_index() didn't return false");

    status = vector_delete_index(vector, 4);
    tests_log_test(!status, "vector_delete_at_index() didn't return false");

    status = vector_set_element_at_index(vector, 0, 4);
    tests_log_test(status, "vector_set_element_at_index() didn't return true");

    status = vector_set_element_at_index(vector, 1, 5);
    tests_log_test(status, "vector_set_element_at_index() didn't return true");

    status = vector_set_element_at_index(vector, 2, 6);
    tests_log_test(status, "vector_set_element_at_index() didn't return true");

    status = vector_element_at_index(vector, 0, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 4, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 1, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 5, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 2, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 6, "vector_element_at_index() gave wrong value");

    sz = vector_length(vector);
    tests_log_test(sz == 3, "vector_length() gave wrong value");

    status = vector_is_empty(vector);
    tests_log_test(!status, "vector_is_empty() gave wrong value");

    status = vector_prepend(vector, 3);
    tests_log_test(status, "vector_prepend() didn't return true");

    status = vector_element_at_index(vector, 0, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 3, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 1, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 4, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 2, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 5, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 3, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 6, "vector_element_at_index() gave wrong value");

    sz = vector_length(vector);
    tests_log_test(sz == 4, "vector_length() gave wrong value");

    sz = vector_capacity(vector);
    tests_log_test(sz == 6, "vector_capacity() gave wrong value");

    sz = vector_free_space(vector);
    tests_log_test(sz == 2, "vector_free_space() gave wrong value");

    status = vector_is_empty(vector);
    tests_log_test(!status, "vector_is_empty() gave wrong value");
    
    status = vector_insert(vector, 2, 7);
    tests_log_test(status, "vector_insert_index() didn't return true");

    status = vector_element_at_index(vector, 0, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 3, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 1, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 4, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 2, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 7, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 3, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 5, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 4, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 6, "vector_element_at_index() gave wrong value");

    sz = vector_length(vector);
    tests_log_test(sz == 5, "vector_length() gave wrong value");

    sz = vector_capacity(vector);
    tests_log_test(sz == 6, "vector_capacity() gave wrong value");

    sz = vector_free_space(vector);
    tests_log_test(sz == 1, "vector_free_space() gave wrong value");

    status = vector_is_empty(vector);
    tests_log_test(!status, "vector_is_empty() gave wrong value");
    
    status = vector_append(vector, 8);
    tests_log_test(status, "vector_append() didn't return true");

    status = vector_element_at_index(vector, 0, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 3, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 1, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 4, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 2, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 7, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 3, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 5, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 4, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 6, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 5, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 8, "vector_element_at_index() gave wrong value");

    sz = vector_length(vector);
    tests_log_test(sz == 6, "vector_length() gave wrong value");

    sz = vector_capacity(vector);
    tests_log_test(sz == 6, "vector_capacity() gave wrong value");

    sz = vector_free_space(vector);
    tests_log_test(sz == 0, "vector_free_space() gave wrong value");

    status = vector_is_empty(vector);
    tests_log_test(!status, "vector_is_empty() gave wrong value");
    
    status = vector_delete_front(vector);
    tests_log_test(status, "vector_delete_front() failed");

    status = vector_delete_back(vector);
    tests_log_test(status, "vector_delete_back() failed");

    status = vector_delete_index(vector, 2);
    tests_log_test(status, "vector_delete_back() failed");

    status = vector_element_at_index(vector, 0, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 4, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 1, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 7, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 2, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 6, "vector_element_at_index() gave wrong value");

    sz = vector_length(vector);
    tests_log_test(sz == 3, "vector_length() gave wrong value");

    sz = vector_capacity(vector);
    tests_log_test(sz == 6, "vector_capacity() gave wrong value");

    sz = vector_free_space(vector);
    tests_log_test(sz == 3, "vector_free_space() gave wrong value");

    status = vector_is_empty(vector);
    tests_log_test(!status, "vector_is_empty() gave wrong value");
    
    status = vector_set_element_at_index(vector, 0, 11);
    tests_log_test(status, "vector_set_element_at_index() didn't return true");

    status = vector_set_element_at_index(vector, 2, 9);
    tests_log_test(status, "vector_set_element_at_index() didn't return true");

    status = vector_element_at_index(vector, 0, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 11, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 1, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 7, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 2, &n);
    tests_log_test(status, "vector_element_at_index() didn't return true");
    tests_log_test(n == 9, "vector_element_at_index() gave wrong value");

    sz = vector_length(vector);
    tests_log_test(sz == 3, "vector_length() gave wrong value");

    sz = vector_capacity(vector);
    tests_log_test(sz == 6, "vector_capacity() gave wrong value");

    sz = vector_free_space(vector);
    tests_log_test(sz == 3, "vector_free_space() gave wrong value");

    status = vector_is_empty(vector);
    tests_log_test(!status, "vector_is_empty() gave wrong value");
    
    vector_destroy(vector);
}

static void test_vector_free_strings(void)
{
    Vector vector = vector_create(1, DATATYPE_STRING, GDS_FREE_ON_DESTROY);
    if ( !vector ) {
        perror("couldn't create vector");
        exit(EXIT_FAILURE);
    }

    bool status;
    size_t sz;
    char * pc;

    sz = vector_length(vector);
    tests_log_test(sz == 1, "vector_length() gave wrong value");

    sz = vector_capacity(vector);
    tests_log_test(sz == 1, "vector_length() gave wrong value");

    status = vector_append(vector, strdup("First string"));
    tests_log_test(status, "vector_append() failed");

    sz = vector_length(vector);
    tests_log_test(sz == 2, "vector_length() gave wrong value");

    sz = vector_capacity(vector);
    tests_log_test(sz == 2, "vector_length() gave wrong value");

    status = vector_append(vector, strdup("Second string"));
    tests_log_test(status, "vector_append() failed");

    sz = vector_length(vector);
    tests_log_test(sz == 3, "vector_length() gave wrong value");

    sz = vector_capacity(vector);
    tests_log_test(sz == 4, "vector_length() gave wrong value");

    status = vector_append(vector, strdup("Third string"));
    tests_log_test(status, "vector_append() failed");

    sz = vector_length(vector);
    tests_log_test(sz == 4, "vector_length() gave wrong value");

    sz = vector_capacity(vector);
    tests_log_test(sz == 4, "vector_length() gave wrong value");

    sz = vector_free_space(vector);
    tests_log_test(sz == 0, "vector_free_space() gave wrong value");

    status = vector_is_empty(vector);
    tests_log_test(!status, "vector_is_empty() gave wrong value");
    
    status = vector_element_at_index(vector, 0, &pc);
    tests_log_test(status, "vector_element_index() failed");
    tests_log_test(pc == NULL, "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 1, &pc);
    tests_log_test(status, "vector_element_index() failed");
    tests_log_test(!strcmp(pc, "First string"),
                   "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 2, &pc);
    tests_log_test(status, "vector_element_index() failed");
    tests_log_test(!strcmp(pc, "Second string"),
                   "vector_element_at_index() gave wrong value");

    status = vector_element_at_index(vector, 3, &pc);
    tests_log_test(status, "vector_element_index() failed");
    tests_log_test(!strcmp(pc, "Third string"),
                   "vector_element_at_index() gave wrong value");

    status = vector_delete_index(vector, 2);
    tests_log_test(status, "vector_delete_index() failed");

    status = vector_element_at_index(vector, 2, &pc);
    tests_log_test(status, "vector_element_index() failed");
    tests_log_test(!strcmp(pc, "Third string"),
                   "vector_element_at_index() gave wrong value");

    sz = vector_length(vector);
    tests_log_test(sz == 3, "vector_length() gave wrong value");

    sz = vector_capacity(vector);
    tests_log_test(sz == 4, "vector_length() gave wrong value");

    sz = vector_free_space(vector);
    tests_log_test(sz == 1, "vector_free_space() gave wrong value");

    status = vector_is_empty(vector);
    tests_log_test(!status, "vector_is_empty() gave wrong value");
    
    vector_destroy(vector);
}

static void test_vector_find(void)
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

    bool status;
    size_t index;

    status = vector_find(vector, &index, 10ULL);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 0, "vector_find() gave wrong index");

    status = vector_find(vector, &index, 20ULL);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 1, "vector_find() gave wrong index");

    status = vector_find(vector, &index, 40ULL);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 2, "vector_find() gave wrong index");

    status = vector_find(vector, &index, 30ULL);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 3, "vector_find() gave wrong index");

    status = vector_find(vector, &index, 50ULL);
    tests_log_test(!status, "vector_find() incorrectly found element");

    vector_destroy(vector);
}

struct hms {
    int hour;
    int minute;
    int second;
};

static int compare_hms(const void * s1, const void * s2)
{
    const struct hms * hms1 = *((const struct hms **) s1);
    const struct hms * hms2 = *((const struct hms **) s2);

    int hours_comp, minutes_comp, seconds_comp;

    if ( hms1->hour < hms2->hour ) {
        hours_comp = -1;
    }
    else if ( hms1->hour > hms2->hour ) {
        hours_comp = 1;
    }
    else {
        hours_comp = 0;
    }

    if ( hms1->minute < hms2->minute ) {
        minutes_comp = -1;
    }
    else if ( hms1->minute > hms2->minute ) {
        minutes_comp = 1;
    }
    else {
        minutes_comp = 0;
    }

    if ( hms1->second < hms2->second ) {
        seconds_comp = -1;
    }
    else if ( hms1->second > hms2->second ) {
        seconds_comp = 1;
    }
    else {
        seconds_comp = 0;
    }

    if ( !hours_comp && !minutes_comp && !seconds_comp ) {
        return 0;
    }
    else {
        if ( hours_comp ) {
            return hours_comp;
        }
        else if ( minutes_comp ) {
            return minutes_comp;
        }
        else {
            return seconds_comp;
        }
    }
}

void test_vector_find_struct(void)
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

    bool status;
    size_t index;

    status = vector_find(vector, &index, (void *) &h1);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 0, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (void *) &h2);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 1, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (void *) &h3);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 2, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (void *) &h4);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 3, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (void *) &h5);
    tests_log_test(!status, "vector_find() incorrectly found element");

    vector_destroy(vector);
}

static void test_vector_sort_strings(void)
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

    bool status;
    size_t index;

    status = vector_find(vector, &index, "Elephant");
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 0, "vector_find() gave wrong index");

    status = vector_find(vector, &index, "Dog");
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 1, "vector_find() gave wrong index");

    status = vector_find(vector, &index, "Giraffe");
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 2, "vector_find() gave wrong index");

    status = vector_find(vector, &index, "Aardvark");
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 3, "vector_find() gave wrong index");

    status = vector_find(vector, &index, "Pelican");
    tests_log_test(!status, "vector_find() incorrectly found element");

    vector_sort(vector);

    status = vector_find(vector, &index, "Aardvark");
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 0, "vector_find() gave wrong index");

    status = vector_find(vector, &index, "Dog");
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 1, "vector_find() gave wrong index");

    status = vector_find(vector, &index, "Elephant");
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 2, "vector_find() gave wrong index");

    status = vector_find(vector, &index, "Giraffe");
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 3, "vector_find() gave wrong index");

    vector_destroy(vector);
}

static void test_vector_sort_sizet(void)
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

    bool status;
    size_t index;

    status = vector_find(vector, &index, (size_t) 100);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 0, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (size_t) 400);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 1, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (size_t) 200);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 2, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (size_t) 300);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 3, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (size_t) 500);
    tests_log_test(!status, "vector_find() incorrectly found element");

    vector_sort(vector);

    status = vector_find(vector, &index, (size_t) 100);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 0, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (size_t) 200);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 1, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (size_t) 300);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 2, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (size_t) 400);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 3, "vector_find() gave wrong index");

    vector_destroy(vector);
}

static void test_vector_sort_struct(void)
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

    bool status;
    size_t index;

    status = vector_find(vector, &index, (void *) &h3);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 0, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (void *) &h2);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 1, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (void *) &h1);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 2, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (void *) &h4);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 3, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (void *) &h5);
    tests_log_test(!status, "vector_find() incorrectly found element");

    vector_sort(vector);

    status = vector_find(vector, &index, (void *) &h1);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 0, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (void *) &h2);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 1, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (void *) &h3);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 2, "vector_find() gave wrong index");

    status = vector_find(vector, &index, (void *) &h4);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 3, "vector_find() gave wrong index");

    vector_destroy(vector);
}

void test_vector_reverse_sort(void)
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

    bool status;
    size_t index;

    status = vector_find(vector, &index, 100L);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 0, "vector_find() gave wrong index");

    status = vector_find(vector, &index, 400L);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 1, "vector_find() gave wrong index");

    status = vector_find(vector, &index, 200L);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 2, "vector_find() gave wrong index");

    status = vector_find(vector, &index, 300L);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 3, "vector_find() gave wrong index");

    vector_reverse_sort(vector);

    status = vector_find(vector, &index, 400L);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 0, "vector_find() gave wrong index");

    status = vector_find(vector, &index, 300L);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 1, "vector_find() gave wrong index");

    status = vector_find(vector, &index, 200L);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 2, "vector_find() gave wrong index");

    status = vector_find(vector, &index, 100L);
    tests_log_test(status, "vector_find() failed to find element");
    tests_log_test(index == 3, "vector_find() gave wrong index");

    vector_destroy(vector);
}

void test_vector(void)
{
    test_vector_basic();
    test_vector_free_strings();
    test_vector_find();
    test_vector_find_struct();
    test_vector_sort_strings();
    test_vector_sort_sizet();
    test_vector_sort_struct();
    test_vector_reverse_sort();
}
