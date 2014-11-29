/*  Sample program to demonstrate GDS error reporting macros  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "gds_util.h"

static void print_usage(void)
{
    printf("Usage: errormacros [OPTIONS]\n\n");
    printf("Illustrates the use of several general purpose error reporting\n");
    printf("macros defined by the GDS library.\n\n");
    printf("Options:\n");
    printf("  -h, --help    Output this help message\n");
    printf("  vs            Illustrate variable argument strerror() macro\n");
    printf("  s             Illustrate fixed argument strerror() macro\n");
    printf("  ve            Illustrate variable argument error macro\n");
    printf("  e             Illustrate fixed argument error macro\n");
}

int main(int argc, char ** argv)
{
    if ( argc != 2 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help") ) {
        print_usage();
        return EXIT_SUCCESS;
    }

    if ( !strcmp(argv[1], "vs") ) {
        errno = 14;
        quit_strerror("errormacros", "%d apples and %.2f oranges", 5, 2.34);
    }

    if ( !strcmp(argv[1], "s") ) {
        errno = 13;
        quit_strerror("errormacros", "apples and oranges");
    }

    if ( !strcmp(argv[1], "ve") ) {
        quit_error("errormacros", "%d dogs and %.2f cats", 100, 3.14);
    }

    if ( !strcmp(argv[1], "e") ) {
        quit_error("errormacros", "apples and oranges");
    }

    print_usage();

    return 0;
}
