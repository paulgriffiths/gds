/*  Interface to struct comparison function used for unit tests  */

#ifndef PG_GENERIC_DATA_STRUCTURES_TEST_STRUCT_H
#define PG_GENERIC_DATA_STRUCTURES_TEST_STRUCT_H

/*  Struct used for struct element finding  */

struct hms {
    int hour;
    int minute;
    int second;
};

int compare_hms(const void * s1, const void * s2);

#endif      /*  PG_GENERIC_DATA_STRUCTURES_TEST_STRUCT_H  */
