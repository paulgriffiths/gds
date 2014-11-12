#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_util.h"
#include "dict.h"

#define BUFFER_LEN 1024

void print_usage(void)
{
    printf("Usage: ifreader [FILE] [KEY] ...\n\n");
    printf("Reads a file containing 'key=value' strings on separate lines,\n");
    printf("and outputs the values for the specified keys. Blank lines and\n");
    printf("lines beginning with '#' in the file are ignored.\n\n");
    printf("Options:\n");
    printf("  -h, --help        Output this help message\n");
}

int main(int argc, char ** argv)
{
    if ( argc < 3 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help") ) {
        print_usage();
        return EXIT_SUCCESS;
    }

    FILE * fp = fopen(argv[1], "r");
    if ( !fp ) {
        perror("couldn't open file for reading");
        return EXIT_FAILURE;
    }

    Dict keys = dict_create(DATATYPE_STRING, GDS_FREE_ON_DESTROY);
    char buffer[BUFFER_LEN];
    while ( fgets(buffer, BUFFER_LEN, fp) ) {
        trim(buffer);
        if ( *buffer != 0 && *buffer != '#' ) {
            struct pair_string * pair = pair_string_create(buffer, '=');
            if ( !pair ) {
                perror("couldn't create string pair");
                return EXIT_FAILURE;
            }

            trim(pair->first);
            trim(pair->second);

            dict_insert(keys, pair->first, pg_strdup(pair->second));
            pair_string_destroy(pair);
        }
    }

    size_t i = 2;
    while ( argv[i] ) {
        char * value;
        if ( dict_value_for_key(keys, argv[i], &value) ) {
            printf("'%s' is defined with value '%s'\n", argv[i], value);
        }
        else {
            printf("'%s' is not defined\n", argv[i]);
        }
        ++i;
    }

    if ( fclose(fp) ) {
        perror("couldn't close file");
        return EXIT_FAILURE;
    }

    dict_destroy(keys);

    return 0;
}
