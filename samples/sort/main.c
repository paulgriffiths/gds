/*
 * sort
 * ====
 *
 * Demonstration program for generic list data structure.
 *
 * Sorts lines from a file, or from standard input.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pggds/gds_util.h>
#include <pggds/list.h>
#include <pggds/gds_string.h>

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
                fp = xfopen(argv[index], "r");
                fromfile = true;
            }
        }
    }

    /*  Set input to standard input if no file was specified  */

    if ( !fp ) {
        fp = stdin;
    }

    /*  Read all available lines and store them in the list  */

    List list = list_create(DATATYPE_GDSSTRING,
                            GDS_FREE_ON_DESTROY | GDS_EXIT_ON_ERROR, 0);

    GDSString line;
    while ( (line = gds_str_getline(BUFFER_SIZE, fp)) ) {
        list_append(list, line);
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
        GDSString str;
        list_get_value_itr(itr, &str);
        printf("%3zu: %s\n", i++, gds_str_cstr(str));
        itr = list_itr_next(itr);
    }

    /*  Clean up and exit  */

    list_destroy(list);

    if ( fromfile ) {
        fclose(fp);
    }

    return 0;
}
