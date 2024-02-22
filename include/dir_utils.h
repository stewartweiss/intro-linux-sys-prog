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

#ifndef DIR_UTILS_H
#define DIR_UTILS_H

#include "common_hdrs.h"
#include <dirent.h>



/** isdir(dp) returns TRUE if the dirent structure to which it points
*   is a  directory, and FALSE if it isn't. It assumes that the dirent*
*   argument is a valid pointer returned by a call to readdir() or telldir(),
*   or visited by scandir().
*/
BOOL isdir( const struct dirent *direntp);

/** dirsfirstsort(&d1, &d2) compares the direct structures pointed to by
*   d1 and d2 and returns -1, 0, or 1 according to the sort order:
*   if both are directories, return
*       -1 if d1 <= d2 by the string collating order
*        1 if d2 > d1 by the string collating order
*   else
*       -1  if d1 is a directory and d2 is not
*        1  if d1 is not a directory and d2 is
*   else if neither is a directory, return -1, 0, or 1 if d1 < d2,
*         d1 == d2, or d1 > d2 respectively.
*/
int dirsfirstsort(const struct dirent **a, const struct dirent **b);

#endif /* DIR_UTILS_H */
