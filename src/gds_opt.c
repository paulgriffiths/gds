/*!
 * \file            gds_opt.c
 * \brief           Implementation of command line option functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#define GDSDEBUG

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <pggds/gds_util.h>
#include <pggds/dict.h>
#include <pggds/vector.h>
#include <pggds/gds_string.h>

/*!  Enumeration type for option argument  */
enum gds_argument_type {
    GDS_ARGUMENT_NO,    /*!<  Option does not take an argument          */
    GDS_ARGUMENT_YES,   /*!<  Option takes an argument                  */
};

/*!  File scope variable to hold program name  */
static const char * progname = NULL;

/*!  File scope variable to signify if command line has been parsed  */
static bool parsed = false;

/*!  File scope dictionary to hold parsed options  */
static Dict options = NULL;

/*!  File scope vector to hold non-option arguments  */
static Vector nonopts = NULL;

/*!
 * \brief           Returns a dictionary of recognized options.
 * \details         The dictionary is created to match the provided string.
 * \param allowed   A string representation of the allowed options. Each
 * option should be represented by a single alphabetic character. Each
 * option can optionally be followed by a ':' to show that it can accept
 * an argument.
 * \returns         A dictionary containing the recognized options.
 */
static Dict gds_get_recognized_options(const char * allowed);

/*!
 * \brief           Creates the static structures used by the module.
 */
static bool create_static_structures(void);

/*!
 * \brief           Destroys the static structures used by the module.
 */
static void destroy_static_structures(void);

bool gds_parse_options(const char * allowed, char ** argv)
{
    parsed = false;
    if ( *argv == NULL ) {

        /*  Don't try to parse an empty argv list  */

        log_error("gds library", "memory allocation failed");
        return false;
    }

    /*  Get a dictionary of the recognized options  */

    Dict recognized = gds_get_recognized_options(allowed);
    if ( !recognized ) {
        progname = NULL;
        return false;
    }

    /*  Destroy existing static structures, if necessary,
     *  and create fresh ones for the first or new parse  */

    destroy_static_structures();
    if ( !create_static_structures() ) {
        dict_destroy(recognized);
        return false;
    }

    bool error = false;
    progname = *argv++;

    /*  Loop through each command line argument  */

    while ( !error && *argv ) {
        GDSString curopt = gds_str_create(*argv); 
        if ( !curopt ) {
            log_error("gds library", "memory allocation failed");
            error = true;
        }
        else {
            if ( gds_str_length(curopt) == 0 ) {

                /*  Don't accept empty arguments  */

                error = true;
            }
            else if ( gds_str_char_at_index(curopt, 0) == '-' ) {
                
                /*  Argument looks like an option  */

                if ( gds_str_length(curopt) < 2 ) {

                    /*  The actual option is missing, so stop  */

                    error = true;
                }
                else {

                    /*  We have a good option  */

                    char opt = gds_str_char_at_index(curopt, 1);
                    char optname[2] = {0};
                    optname[0] = opt;

                    if ( dict_has_key(recognized, optname) ) {

                        /*  Option is recognized, check for argument  */

                        int argtype;
                        dict_value_for_key(recognized, optname, &argtype);
                        if ( argtype == GDS_ARGUMENT_NO ) {
                            if ( gds_str_length(curopt) > 2 ) {
                                
                                /*  An argument was provided, reject it  */

                                error = true;
                            }
                            else {
                                if ( !dict_insert(options, optname, NULL) ) {
                                    error = true;
                                }
                            }
                        }
                        else if ( argtype == GDS_ARGUMENT_YES ) {
                            
                            /*  Argument will be the empty string if one
                             *  was not provided, this is the desired
                             *  behavior.                                 */

                            GDSString arg = gds_str_substr_right(curopt,
                                    gds_str_length(curopt) - 2);
                            if ( !arg ) {
                                error = true;
                            }
                            else {
                                if ( !dict_insert(options, optname, arg) ) {
                                    error = true;
                                }
                            }
                        }
                    }
                    else {

                        /*  Option is not recognized, reject it  */

                        error = true;
                    }
                }
            }
            else {

                /*  Argument was not an option  */

                char * c = gds_strdup(gds_str_cstr(curopt));
                if ( !c ) {
                    log_error("gds library", "memory allocation failed");
                    error = true;
                }
                else {
                    if ( !vector_append(nonopts, c) ) {
                        error = true;
                    }
                }
            }

            gds_str_destroy(curopt);
        }

        ++argv;
    }

    /*  Clean up and exit  */

    dict_destroy(recognized);

    if ( error ) {
        destroy_static_structures();
        parsed = false;
    }
    else {
        parsed = true;
    }

    return parsed;
}

void gds_free_options(void)
{
    if ( options ) {
        dict_destroy(options);
        options = NULL;
    }

    if ( nonopts ) {
        vector_destroy(nonopts);
        nonopts = NULL;
    }

    progname = NULL;
}

const char * gds_option_progname(void)
{
    return progname;
}

bool gds_option_present(const char * optname)
{
    if ( parsed ) {
        if ( dict_has_key(options, optname) ) {
            return true;
        }
    }
    return false;
}

const char * gds_option_argument_string(const char * optname)
{
    if ( parsed ) {
        if ( dict_has_key(options, optname) ) {
            GDSString arg;
            dict_value_for_key(options, optname, &arg);
            if ( arg ) {
                return gds_str_cstr(arg);
            }
            else {
                return NULL;
            }
        }
    }
    return NULL;
}

bool gds_option_argument_int(const char * optname, int * value)
{
    if ( parsed ) {
        if ( dict_has_key(options, optname) ) {
            GDSString arg;
            dict_value_for_key(options, optname, &arg);
            if ( arg ) {
                int n;
                if ( gds_str_intval(arg, 0, &n) ) {
                    if ( value ) {
                        *value = n;
                    }
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }
    }

    return false;
}

int gds_option_nonopts_number(void)
{
    if ( parsed ) {
        return vector_length(nonopts);
    }
    else {
        return 0;
    }
}

const char * gds_option_nonopt(const size_t index)
{
    if ( parsed ) {
        if ( index >= vector_length(nonopts) ) {
            return NULL;
        }
        else {
            char * nonopt;
            vector_element_at_index(nonopts, index, &nonopt);
            return nonopt;
        }
    }
    else {
        return NULL;
    }
}

static bool create_static_structures(void)
{
    options = dict_create(DATATYPE_GDSSTRING, GDS_FREE_ON_DESTROY);
    if ( !options ) {
        progname = NULL;
        return false;
    }

    nonopts = vector_create(0, DATATYPE_STRING, GDS_FREE_ON_DESTROY);
    if ( !nonopts ) {
        dict_destroy(options);
        progname = NULL;
        return false;
    }

    return true;
}

static void destroy_static_structures(void)
{
    if ( options ) {
        dict_destroy(options);
        options = NULL;
        progname = NULL;
    }

    if ( nonopts ) {
        vector_destroy(nonopts);
        nonopts = NULL;
        progname = NULL;
    }
}

static Dict gds_get_recognized_options(const char * allowed)
{
    Dict recognized = dict_create(DATATYPE_INT, GDS_FREE_ON_DESTROY);
    if ( !recognized ) {
        return NULL;
    }

    char last_option = 0;
    char opt_name[2] = {0};
    char current_option;
    bool error = false;

    /*  Loop through characters in the provided string  */

    while ( !error && (current_option = *allowed) ) {
        if ( current_option == ':' ) {
            if ( last_option ) {

                /*  Argument character follows an option, so store 
                 *  the option in the dictionary specifying that it
                 *  accepts an argument                              */

                opt_name[0] = last_option;
                if ( !dict_insert(recognized, opt_name, GDS_ARGUMENT_YES) ) {
                    error = true;
                }
                last_option = 0;
            }
            else {

                /*  Argument character follows a non-option
                 *  character, so reject it.                 */

                error = true;
            }
        }
        else if ( isalpha(current_option) ) {
            if ( last_option ) {

                /*  The last character was an option, and this the
                 *  current character is not the argument character,
                 *  the previous option does not take an argument, so
                 *  store it in the dictionary specifying as such.     */

                opt_name[0] = last_option;
                if ( !dict_insert(recognized, opt_name, GDS_ARGUMENT_NO) ) {
                    error = true;
                }
            }

            last_option = current_option;
        }
        else {

            /*  Unallowable character, reject it.  */

            error = true;
        }

        ++allowed;
    }

    /*  Make sure we get the last option, if present  */

    if ( !error && last_option ) {
        opt_name[0] = last_option;
        if ( !dict_insert(recognized, opt_name, GDS_ARGUMENT_NO) ) {
            error = true;
        }
    }

    /*  Clean up and exit  */

    if ( error ) {
        dict_destroy(recognized);
        return NULL;
    }
    else {
        return recognized;
    }
}
