#include "../include/common_hdrs.h"
#include "../include/get_nums.h"



int  get_int(char *arg, int flags, int *value, char *msg )
{
    long val;
    int res =  get_long(arg, flags, &val, msg );

    if ( VALID_NUMBER == res ) {
        if ( val > INT_MAX ) {
            sprintf(msg, "%ld is out of range\n", val);
            return OUT_OF_RANGE;
        }
        else if ( val < INT_MIN ) {
            sprintf(msg, "%ld is out of range\n", val);
            return OUT_OF_RANGE;
            }
        else {
            *value = val;
            return VALID_NUMBER;
        }
    }
    else { /* get_long failed in one way or another */
        return res;
    }
}
