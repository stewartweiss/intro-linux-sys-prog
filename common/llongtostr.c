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

void lltostr(long long int num, char* str, int width)
{
    int i = 1;
    BOOL negative = FALSE;
    int  digit;

    if ( num < 0 ) {
        negative = TRUE;
        num = -num;
    }
    str[width-1] = '\0';
    if ( num == 0 ) {
       str[width -2] = '0';
       i = 2;
    }
    else {
        while ( num > 0 ) {
            digit = num % 10;
            num   = num/10;
            if ( width -1 -i >= 0 ) {
                str[width -1 -i] = '0' + digit;
                i++;
            }
            else
                break;
        }
        if ( negative ) {
            str[width -1 -i] = '-';
            i++;
        }
    }
    while ( width -1 -i >= 0 ) {
        str[width -1 -i] = ' ';
        i++;
    }
}
