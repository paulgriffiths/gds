/*
 * sort
 * ====
 *
 * Demonstration program for generic list data structure.
 *
 * Sorts lines from a file, or from standard input.
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "list.h"

#define BUFFER_SIZE 1024

void print_help_message(void)
{
    printf("Usage: sort [FILE]\n\n");
    printf("Options:\n");
    printf("  -r, --reverse    sort in reverse\n");
    printf("  -h, --help       show this help message.\n\n");
    printf("With no FILE, read standard input.\n");
}

int main(int argc, char ** argv)
{
    bool reverse = false;
    bool fromfile = false;
    FILE * fp = NULL;

    /*  Read command line options, if any  */

    if ( argc > 1 ) {
        size_t index = 0;
        while ( argv[++index] ) {
            if ( !strcmp(argv[index], "-h") ||
                 !strcmp(argv[index], "--help") ) {
                print_help_message();

                if ( fp ) {

                    /*  In case a filename was specified
                     *  before -h on the command line.    */

                    fclose(fp);
                }

                return EXIT_SUCCESS;
            }
            else if ( !strcmp(argv[index], "-r") ||
                      !strcmp(argv[index], "--reverse") ) {
                reverse = true;
            }
            else {
                if ( !(fp = fopen(argv[index], "r")) ) {
                    fprintf(stderr, "Couldn't open file %s for reading - "
                            "%s (%d)\n", argv[index], strerror(errno),
                            errno);
                    return EXIT_FAILURE;
                }
                fromfile = true;
            }
        }
    }

    /*  Set input to standard input if no file was specified  */

    if ( !fp ) {
        fp = stdin;
    }

    /*  Read all available lines and store them in the list  */

    List list = list_create(DATATYPE_STRING,
                            GDS_FREE_ON_DESTROY | GDS_EXIT_ON_ERROR, 0);

    char buffer[BUFFER_SIZE];
    while ( fgets(buffer, sizeof buffer, fp) ) {
        const size_t len = strlen(buffer);
        if ( buffer[len - 1] == '\n' ) {

            /*  Remove trailing newline  */

            buffer[len - 1] = 0;
        }

        char * pc = strdup(buffer);
        if ( !pc ) {
            fprintf(stderr, "Couldn't allocate memory - %s (%d)\n",
                    strerror(errno), errno);
            return EXIT_FAILURE;
        }

        list_append(list, pc);
    }

    /*  Sort list  */

    if ( reverse ) {
        list_reverse_sort(list);
    }
    else {
        list_sort(list);
    }

    /*  Output sorted lines  */

    ListItr itr = list_itr_first(list);
    size_t i = 1;
    while ( itr ) {
        char * str;
        list_get_value_itr(itr, &str);
        printf("%3zu: %s\n", i++, str);
        itr = list_itr_next(itr);
    }

    /*  Clean up and exit  */

    list_destroy(list);

    if ( fromfile ) {
        fclose(fp);
    }

    return 0;
}
