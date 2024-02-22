

#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

#define  STRING_MAX  1024

/* Create a BOOL type */
#ifdef FALSE
    #undef FALSE
#endif
#ifdef TRUE
    #undef TRUE
#endif

#ifdef BOOL
    #undef BOOL
#endif
typedef enum{FALSE, TRUE} BOOL;



#endif /* COMMON_DEFS_H */
