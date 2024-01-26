
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
#include <dirent.h>

BOOL isdir(  const struct dirent *direntp)
{
#ifdef _DIRENT_HAVE_D_TYPE
    if( direntp->d_type  == DT_DIR)
#else
    struct stat    statbuf;
    stat(direntp->d_name, &statbuf);
    if ( S_ISDIR(statbuf.st_mode) )
#endif
        return TRUE;
    else
        return FALSE;
}


int dirsfirstsort(const struct dirent **a, const struct dirent **b)
{
    if ( isdir(*a) )
        if (! isdir(*b) )
            return -1;
        else
            return (alphasort(a,b));
    else
        if ( isdir(*b) )
            return 1;
        else
            return (alphasort(a,b));
}
