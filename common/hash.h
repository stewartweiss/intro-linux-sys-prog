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

#ifndef HASH_TABLE_H__
#define HASH_TABLE_H__



#include "common_hdrs.h"

// The INITIAL_SIZE should be large enough that it will not need to be
// resized, but you might want to implement resizing in your class.
#define ACTIVE        1
#define DELETED       2
#define EMPTYTAG      3



typedef unsigned long long  hash_val;

typedef struct hash_entry_tag
{
    hash_val   item;
    short      state;
} hash_entry;

typedef  struct hashtable_tag
{
    hash_entry*  table;
    int          table_size;
    int          current_size;
    int          numbits_to_shift;
}  hash_table;



void init_hash ( hash_table *htable, int initial_size  );
void rehash     ( hash_table *h, unsigned int new_size );
int  findloc_hash( hash_table h, hash_val val);
BOOL is_in_hash  ( hash_table h, hash_val val);
BOOL insert_hash ( hash_table *h, hash_val val );
void free_hash(hash_table* htable);



#endif /* __HASH_TABLE_H__ */

