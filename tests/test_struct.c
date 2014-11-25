/*  Implementation of struct comparison function used for unit tests  */

#include "test_struct.h"

/*  Comparison function used for struct element finding  */

int compare_hms(const void * s1, const void * s2)
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
