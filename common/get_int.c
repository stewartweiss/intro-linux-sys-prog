/*****************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.lgplv3 for details.               *
*****************************************************************************/


#include "common_hdrs.h"
#include "get_nums.h"



int  get_int(char *arg, int flags, int *value, char *msg )
{
    long val;
    int res =  get_long(arg, flags, &val, msg );

    if ( VALID_NUMBER == res ) {
        if ( val > INT_MAX  || val < INT_MIN ) {
            if ( NULL != msg )
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
