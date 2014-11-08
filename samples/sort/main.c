#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include "vector.h"

int main(void)
{
    Vector vector = vector_create(1, DATATYPE_STRING,
                                  GDS_FREE_ON_DESTROY | GDS_EXIT_ON_ERROR, 0);

    vector_set_element_at_index(vector, 0, strdup("Dog"));
    vector_append(vector, strdup("Aardvark"));
    vector_append(vector, strdup("Monkey"));
    vector_append(vector, strdup("Giraffe"));
    vector_append(vector, strdup("Pelican"));
    vector_append(vector, strdup("Dolphin"));
    vector_append(vector, strdup("Zebra"));

    vector_sort(vector);

    printf("Sorted list is:\n");

    for ( size_t i = 0; i < vector_length(vector); ++i ) {
        char * str;
        vector_element_at_index(vector, i, &str);
        printf("%s\n", str);
    }

    vector_destroy(vector);

    return 0;
}
