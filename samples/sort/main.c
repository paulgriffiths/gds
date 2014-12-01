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
#include <stdbool.h>
#include <pggds/gds_util.h>
#include <pggds/list.h>
#include <pggds/gds_string.h>
#include <pggds/gds_opt.h>

/*  Holds main program parameters  */

struct params {
    bool reverse;
    bool fromfile;
    FILE * fp;
};

/*  Prints a help/usage message  */

static void print_help_message(void)
{
    printf("Usage: sort [FILE]\n\n");
    printf("Options:\n");
    printf("  -r    sort in reverse\n");
    printf("  -h    show this help message.\n\n");
    printf("With no FILE, read standard input.\n");
}

/*  Parses the command line - returns false if program should exit  */

static bool parse_command_line(struct params * params, char ** argv)
{
    bool cont = true;

    if ( !gds_parse_options("hr", argv) ) {
        quit_error("sort", "couldn't parse command line");
    }

    if ( gds_option_present("h") ) {
        print_help_message();
        cont = false;
    }
    else {
        if ( gds_option_present("r") ) {
            params->reverse = true;
        }
        else {
            params->reverse = false;
        }

        if ( gds_option_nonopts_number() > 0 ) {
            params->fp = xfopen(gds_option_nonopt(0), "r");
            params->fromfile = true;
        }
        else {
            params->fp = stdin;
            params->fromfile = false;
        }
    }

    gds_free_options();
    return cont;
}

/*  Reads lines from a file and creates a list from them  */

static List read_lines_from_file(FILE * fp)
{
    List list = list_create(DATATYPE_GDSSTRING,
                            GDS_FREE_ON_DESTROY | GDS_EXIT_ON_ERROR, 0);
    if ( !list ) {
        quit_error("sort", "couldn't create list");
    }

    GDSString line;
    while ( (line = gds_str_getline(128, fp)) ) {
        list_append(list, line);
    }

    return list;
}

/*  Prints a list of strings with numbered lines  */

static void print_list(List list)
{
    ListItr itr = list_itr_first(list);
    size_t i = 1;

    while ( itr ) {
        GDSString str;
        list_get_value_itr(itr, &str);
        printf("%3zu: %s\n", i++, gds_str_cstr(str));
        itr = list_itr_next(itr);
    }
}

/*  Sorts a list, optionally in reverse  */

static void sort_list(List list, const bool reverse)
{
    if ( reverse ) {
        if ( !list_reverse_sort(list) ) {
            quit_error("sort", "couldn't sort list");
        }
    }
    else {
        if ( !list_sort(list) ) {
            quit_error("sort", "couldn't sort list");
        }
    }
}

/*  Main function  */

int main(int argc, char ** argv)
{
    (void) argc;            /*  Ignore unused argument  */

    struct params params;
    if ( !parse_command_line(&params, argv) ) {
        return EXIT_SUCCESS;
    }

    List list = read_lines_from_file(params.fp);
    sort_list(list, params.reverse);
    print_list(list);
    list_destroy(list);

    if ( params.fromfile ) {
        fclose(params.fp);
    }

    return 0;
}
