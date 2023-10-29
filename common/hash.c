

#include "hash.h"
const unsigned long long Hash_Constant = 0.6180339887498948482 * (unsigned long long int ) -1;


void rehash( hash_table *h, unsigned int new_size )
{
    hash_entry*   temp;
    hash_entry*   old_table = h->table;
    int i, s      = new_size;
    int old_size  = h->table_size;
    int size_bits = 0;
    while ( 0 < (s = (s >> 1)) )
        size_bits++;
    temp           = malloc(new_size*sizeof(hash_entry));
    if ( temp == NULL )
        fatal_error(errno, "malloc() in rehash()");

    /* Initialize new hash table. */
    for (i = 0; i < new_size; i++)
        temp[i].state = EMPTY;

    h->numbits_to_shift = sizeof(unsigned long int)*8 - size_bits;
    h->current_size     = 0;
    h->table_size       = new_size;

    /* Re-insert all entries into new table. */
    h->table = temp;
    for ( i = 0; i < old_size; i++)
        if ( old_table[i].state == ACTIVE )
            insert_hash(h, old_table[i].item);
    free(old_table);
}

void free_hash(hash_table * htable)
{
    free(htable->table);
    htable->current_size = 0;
    htable->table_size   = 0;
}

int findloc_hash( hash_table htable,  hash_val item)
{
    int i = (Hash_Constant * item) >> htable.numbits_to_shift;
    int j = i;
    while ( htable.table[i].state != EMPTY &&
            htable.table[i].item  != item ) {
        i = (i+1) % htable.table_size;
        if ( i == j )
            fatal_error(-1, "Out of room in hash table");
    }
    return i;
}

BOOL is_in_hash( hash_table htable,  hash_val item)
{
    int i = findloc_hash(htable, item);
    if ( htable.table[i].state == ACTIVE )
        return TRUE;
    else
        return FALSE;
}

void init_hash( hash_table *htable, int initial_size  )
{
    int s  = initial_size;
    int size_bits  = 0;
    while ( 0 < (s = (s >> 1)) )
        size_bits++;
    htable->table_size   = initial_size;
    htable-> table       = malloc(htable->table_size*sizeof(hash_entry));
    if ( htable-> table == NULL )
        fatal_error(errno, "malloc");

    /* Initialize hash table. */
    for ( int i = 0; i < htable->table_size; i++)
        htable-> table[i].state = EMPTY;
    htable->numbits_to_shift = sizeof(unsigned long int)*8 - size_bits;
    htable->current_size = 0;
}


BOOL insert_hash( hash_table *h, hash_val item )
{
    int i;
    BOOL inserted = FALSE;

    if ( h->current_size >= h->table_size/2 ) {
        h->table_size *= 2;
        rehash(h, h->table_size);
    }

    i = findloc_hash(*h, item);
    if ( h->table[i].state == EMPTY ) {
        h->table[i].state = ACTIVE;
        h->table[i].item = item;
        h->current_size++;
        inserted = TRUE;
    }
    return inserted;
}







