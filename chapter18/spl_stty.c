/*****************************************************************************
  Title          : spl_stty.c
  Author         : Stewart Weiss
  Created on     : July 21, 2024
  Description    :
  Purpose        :
  Usage          : spl_stty [-a | name | name value ]
  Build with     :

*****************************************************************************/

#include   "common_hdrs.h"
#include   "get_nums.h"
#include  <ctype.h>
#include   <termios.h>
#ifndef TIOCGWINSZ
    #include <sys/ioctl.h>
#endif



typedef struct _maskmap
{
    int    mask;  /* Bitmap to access bits of flagset or variable array     */
    char  *name;  /* String name of setting entered on command line         */
} maskmap;


typedef struct _dlymask_map
{
   int  value;   /* Bitmap to access bits of flagset or variable array  */
   int  mask;    /* Multi-bit mask for zeroing bits to be updated       */
   char *name;   /* String name of setting entered on command line     */
} dlymask_map;

typedef struct _baudmap
{
    speed_t baudval;  /* Macro constant that defines a speed setting        */
    char   *name;     /* String representing the numeric value of the speed */
} baudmap;


/* The map of c_iflag flag constants and command-line names: */
const maskmap input_flags[] = {
    {IGNBRK  , "ignbrk" },
    {BRKINT  , "brkint" },
    {IGNPAR  , "ignpar" },
    {PARMRK  , "parmrk" },
    {INPCK   , "inpck" },
    {ISTRIP  , "istrip" },
    {INLCR   , "inlcr" },
    {IGNCR   , "igncr" },
    {ICRNL   , "icrnl" },
    {IXON    , "ixon" },
    {IXOFF   , "ixoff" },
    {IUCLC   , "iuclc" },
    {IXANY   , "ixany" },
    {IMAXBEL , "imaxbel" },
    {IUTF8   , "iutf8" },
    {-1      ,  NULL }
};

/* The map of c_oflag flag constants and command-line names: */
const maskmap output_flags[] = {
    {OPOST  , "opost"} ,
    {OLCUC  , "olcuc"} ,
    {OCRNL  , "ocrnl"} ,
    {ONLCR  , "onlcr"} ,
    {ONOCR  , "onocr"} ,
    {ONLRET , "onlret"},
    {OFILL  , "ofill"},
    {OFDEL  , "ofdel"} ,
    {-1     ,  NULL }
};

/* The map of output symbolic constants, masks, and command-line names: */
const dlymask_map output_dlymasks[] = {
    {BS0    , BSDLY ,"bs0" },
    {BS1    , BSDLY ,"bs1" },
    {CR0    , CRDLY ,"cr0" },
    {CR1    , CRDLY ,"cr1" },
    {CR2    , CRDLY ,"cr2" },
    {CR3    , CRDLY ,"cr3" },
    {FF0    , FFDLY ,"ff0" },
    {FF1    , FFDLY ,"ff1" },
    {NL0    , NLDLY ,"nl0" },
    {NL1    , NLDLY ,"nl1" },
    {VT0    , VTDLY ,"vt0" },
    {VT1    , VTDLY ,"vt1" },
    {TAB0   , TABDLY ,"tab0" },
    {TAB0   , TABDLY ,"-tabs" },
    {TAB1   , TABDLY ,"tab1" },
    {TAB2   , TABDLY ,"tab2" },
    {TAB3   , TABDLY ,"tab3" },
    {TAB3   , TABDLY ,"tabs" },
    {-1     , 0,  NULL }
};

/* The map of c_cflag flag constants and command-line names: */
const maskmap control_flags[] = {
    {PARENB , "parenb"},
    {PARODD , "parodd"},
    {CMSPAR , "cmspar"},
    {HUPCL  , "hupcl"},
    {CSTOPB , "cstopb"},
    {CREAD  , "cread"},
    {CLOCAL , "clocal"},
    {CRTSCTS, "crtscts"},
    {-1     , NULL}
};

/* The map of c_lflag flag constants and command-line names: */
const maskmap local_flags[] = {
    {ISIG , "isig"},
    {ICANON , "icanon"},
    {IEXTEN , "iexten"},
    {ECHO , "echo"},
    {ECHOE , "echoe"},
    {ECHOK , "echok"},
    {ECHONL , "echonl"},
    {NOFLSH , "noflsh"},
    {XCASE , "xcase"},
    {TOSTOP , "tostop"},
    {ECHOPRT , "echoprt"},
    {ECHOCTL , "echoctl"},
    {ECHOKE , "echoke"},
    {FLUSHO , "flusho"},
    {PENDIN , "pendin"},
    { -1     ,  NULL }
};

/* The map of c_cc[] index constants and command-line names: */
const maskmap  cc_vars[] = {
    {VINTR , "intr"},
    {VQUIT , "quit"},
    {VERASE , "erase"},
    {VKILL , "kill"},
    {VEOF , "eof"},
    {VEOL , "eol"},
    {VEOL2 , "eol2"},
    {VSWTC , "swtch"},
    {VSTART , "start"},
    {VSTOP , "stop"},
    {VSUSP , "susp"},
    {VREPRINT , "reprint"},
    {VWERASE , "werase"},
    {VLNEXT , "lnext"},
    {VDISCARD , "discard"},
    { -1,      NULL}
};

const speed_t  NOSUCHBAUD = 9999;
const baudmap baudrates[] = {
    {B0      , "0"  },
    {B50     , "50" },
    {B75     , "75" },
    {B110    , "110"},
    {B134    , "134"},
    {B150    , "150"},
    {B200    , "200"},
    {B300    , "300"},
    {B600    , "600"},
    {B1200   , "1200"},
    {B1800   , "1800"},
    {B2400   , "2400"},
    {B4800   , "4800"},
    {B9600   , "9600"},
    {B19200  , "19200"},
    {B38400  , "38400"},
    {B57600  , "57600"},
    {B115200 , "115200"},
    {B230400 , "230400"},
    {NOSUCHBAUD  , "unknown"}
};



/* Returns the number of zero-bits to the right of the first non-zero bit. */
int rshift( unsigned int mask )
{
    int i = 0;
    while ( (~mask) & 1 ) {
        mask = mask >> 1;
        i++;
    }
    return i;
}

void get_speed(struct termios tt, char* speedstr)
{
    speed_t o_speed, i_speed, speed;

    o_speed = cfgetospeed( &tt);
    i_speed = cfgetispeed( &tt);
    speed = (o_speed > i_speed)? i_speed: o_speed;

    int i = 0;
    while ( baudrates[i].baudval != NOSUCHBAUD)
        if ( baudrates[i].baudval == speed ) {
            strcpy(speedstr,  baudrates[i].name);
            return;
        }
        else
            i++;
    strcpy(speedstr, "unknown");
}

/* Display the baud rate */
void show_speed( struct termios tt )
{
    char  orate[10];
    get_speed(tt, orate);
    printf("speed %s baud; ", orate);
}



void show_brief(struct termios tt)
{
    show_speed(tt);
    printf("line = %d; \n", tt.c_line);
    if ( tt.c_iflag & BRKINT)
        printf("brkint ");
    else
        printf("-brkint ");

    if ( tt.c_iflag & IMAXBEL)
        printf("imaxbel ");
    else
        printf("-imaxbel ");

    if ( tt.c_iflag & IUTF8 )
        printf("iutf8 ");
    else
        printf("-iutf8 ");
    printf("\n");
}

/** show_flagset(flag, map) prints the switch values in flagset flag
 *  using the map passed to it to map macro names to lowercase
 *  names. Disabled flags are prepended with '-'.
 */
void show_flagset(tcflag_t flags, const maskmap* map)
{
    int i = 0;
    while ( map[i].mask >= 0) {
        if ( flags & map[i].mask )
            printf("%s ", map[i].name);
        else
            printf("-%s ", map[i].name);
        i++;
        if ( i == 9 ) printf("\n");
    }
}

void show_odelays( struct termios tt )
{
    printf("bs%d ",  (tt.c_oflag  & BSDLY) >> rshift(BSDLY) );
    printf("ff%d ",  (tt.c_oflag  & FFDLY) >> rshift(FFDLY) );
    printf("cr%d ",  (tt.c_oflag  & CRDLY) >> rshift(CRDLY) );
    printf("tab%d ", (tt.c_oflag  & TABDLY) >> rshift(TABDLY));
    printf("nl%d ",  (tt.c_oflag  & NLDLY) >> rshift(NLDLY) );
    printf("vt%d ",  (tt.c_oflag  & VTDLY) >> rshift(VTDLY) );
}

/** show_csize(tt) prints the string csN, where N is # of digits in a char */
void show_csize( struct termios tt)
{
    int cs_shift = rshift(CSIZE);

    printf("cs%d ",  1 + cs_shift + ((tt.c_cflag  & CSIZE) >> cs_shift));
    return;
}



void show_ccvars( struct termios tt)
{
    int i = 0;
    unsigned char ch;

    while ( cc_vars[i].mask >= 0 ) {
        if ( 0 < (ch = (tt.c_cc[cc_vars[i].mask])) )
            printf("%s = ^%c; ", cc_vars[i].name, (ch -1+'A') & 0x7F);
        else
            printf("%s = <undef>; ", cc_vars[i].name);
        i++;
        if ( i % 6 == 0 ) printf("\n");
    }
}

void show_mintime( struct termios tt)
{
    printf("min = %d; ",  tt.c_cc[VMIN]);
    printf("time = %d; ",  tt.c_cc[VTIME]);
    printf("\n");
}

BOOL  get_window_size( int fd, int *rows, int *cols)
{
    struct winsize size;

    if (ioctl(fd, TIOCGWINSZ,  &size) < 0) {
        printf("Unable to get window size.\n");
        return FALSE;
    }
    *rows = size.ws_row;
    *cols = size.ws_col;
    return TRUE;
}


void  show_window_size( int fd )
{
    int r, c;
    if (! get_window_size(fd, &r, &c) )
        printf("rows <unknown>; columns <unknown>; ");
    else
        printf("rows %d; columns %d; ", r, c);
}

void show_all( int fd,  struct termios ttyinfo)
{
    show_speed( ttyinfo );
    show_window_size( fd );
    printf("line = %d; \n", ttyinfo.c_line);
    show_ccvars( ttyinfo );
    show_mintime( ttyinfo);
    show_flagset(ttyinfo.c_cflag, control_flags);
    show_csize(ttyinfo);
    printf("\n");
    show_flagset(ttyinfo.c_iflag, input_flags);
    printf("\n");
    show_flagset(ttyinfo.c_oflag, output_flags);
    show_odelays(ttyinfo);
    printf("\n");
    show_flagset(ttyinfo.c_lflag, local_flags);
    printf("\n");
}

BOOL set_ccvar(struct termios *tt,  char *name,  unsigned char value)
{
    int i = 0;

    while ( cc_vars[i].mask >= 0 )
        if ( strcmp(cc_vars[i].name, name) )
            i++;
        else {
            tt->c_cc[cc_vars[i].mask] = value;
            return TRUE;
        }
    return FALSE;
}

BOOL set_var(struct termios *tt,  char *name,  char *val)
{
    int   i, number;
    speed_t  speed;
    unsigned char value;
    int   len;

    if ( strcmp(name, "line") == 0 || strcmp(name, "min") == 0 ||
       strcmp(name, "time") == 0 ) {
        if ( VALID_NUMBER != get_int(val, NON_NEG_ONLY, &number, NULL) )
            return FALSE;

        if ( strcmp(name, "line") == 0 ) {
            tt->c_line = number;
            return TRUE;
        }
        if ( strcmp(name, "min") == 0 ) {
            if ( number >= 256 )
                return FALSE;
            tt->c_cc[VMIN] = number;
            return TRUE;
        }
        if ( strcmp(name, "time") == 0 ) {
            if ( number >= 256 )
                return FALSE;
            tt->c_cc[VTIME] = number;
            return TRUE;
        }
    }
    else if ( strcmp(name, "ispeed") == 0 || strcmp(name, "ospeed") == 0 ||
             strcmp(name, "speed") == 0 ) {

        i = 0;
        while ( baudrates[i].baudval != NOSUCHBAUD ) {
            if ( !strcmp(baudrates[i].name, val) ) {
                speed =   baudrates[i].baudval;
                break;
            }
            else
                i++;
        }
        if (baudrates[i].baudval == NOSUCHBAUD )
           return FALSE;

        if ( strcmp(name, "speed") == 0 ) {
            cfsetspeed(tt, speed);
            return TRUE;
        }
        if ( strcmp(name, "ispeed") == 0 ) {
            cfsetispeed(tt, speed);
            return TRUE;
        }
        if ( strcmp(name, "ospeed") == 0 ) {
            cfsetospeed(tt, speed);
            return TRUE;
        }

    }
    else {
        len = strlen(val);
        if ( len <= 2 ) {
            if ( 1 == len  )
                value = val[0];
            else  /* len == 2 */
                if ( (val[0] == '^') && ( isupper(val[1]) ) )
                        value = val[1] - 'A' + 1;
                else
                    return FALSE;
            if ( set_ccvar(tt, name, value) )
                return TRUE;
        }
    }
    return FALSE;
}


BOOL set_switch_in_map(tcflag_t *flags, const maskmap* fmap, char *name, BOOL enable)
{
    int i = 0;
    while ( fmap[i].mask >= 0) {
        if ( strcmp(name, fmap[i].name) == 0 ) {
            if ( enable )
                *flags  |= fmap[i].mask ;
            else
                *flags  &= ~fmap[i].mask ;
            return TRUE;
        }
        i++;
    }
    return FALSE;
}

/* Returns TRUE if name is a valid flag and the flag was updated. */
BOOL  set_switch(struct termios *tt, char *name)
{
    BOOL  state = TRUE;                /* Assume enabling a switch.         */

    if ( name[0] == '-' )              /* Most likely means disable switch. */
        if ( strcmp(name, "-tabs") != 0 ) {  /* Make sure it isn't "-tabs"  */
            state = FALSE;             /* Set state to disable switch.      */
            name = &(name[1]);         /* Search for word after leading '-' */
        }

    if ( set_switch_in_map(&(tt->c_oflag), output_flags, name, state) )
        return TRUE;
    if ( set_switch_in_map(&(tt->c_iflag), input_flags, name, state) )
        return TRUE;
    if ( set_switch_in_map(&(tt->c_lflag), local_flags, name, state) )
        return TRUE;
    if ( set_switch_in_map(&(tt->c_cflag), control_flags, name, state) )
        return TRUE;

    int i = 0;
    while ( output_dlymasks[i].value >= 0) {
        if ( strcmp(name, output_dlymasks[i].name) == 0 ) {
            tt->c_oflag  &= ~output_dlymasks[i].mask;
            tt->c_oflag  |= output_dlymasks[i].value ;
            return TRUE;
        }
        i++;
    }
    return FALSE;
}



int main(int argc, char* argv[])
{
    struct  termios ttyinfo;  /* termios structure to store settings        */
    int     ttyfd;            /* File descriptor for control terminal       */
    int     rows, cols;       /* Window dimensions                          */
    char    speedstr[16];     /* String to store displayed or entered speed */

    /* Get file descriptor for terminal;  if redirected it still gets it.   */
    if ( -1 == (ttyfd = open("/dev/tty", O_RDWR)) )
        fatal_error(errno, "open");

    /* Fill termios structure ttyinfo with current settings.                */
    if ( -1 == tcgetattr( ttyfd , &ttyinfo ) )
        fatal_error(errno, "tcgetattr");


    if ( argc == 1 )  /* No options or arguments -- show brief summary.     */
        show_brief( ttyinfo );

    else if ( argc == 2 ) { /* Several possibilities for this one word */
        if ( ! strcmp(argv[1], "-a") )      /* User entered spl_stty -a    */
            show_all( ttyfd, ttyinfo);
        else if ( strcmp(argv[1], "speed") == 0 ) {
            /* User entered spl_stty speed */
            get_speed( ttyinfo, speedstr); /* Function to get current speed */
            printf("%s\n", speedstr);
        }
        else if ( strcmp(argv[1], "size") == 0 ) {
            /* User entered spl_stty size */
            if ( get_window_size( ttyfd, &rows, &cols ) )
                printf("%d %d\n", rows, cols);
            else
                printf("Unable to retrieve window size\n");
        }
        else { /* User entered either a switch name or a delay constant     */
            if ( set_switch(&ttyinfo, argv[1] ) )
                /* Successfully changed requested setting. */
                tcsetattr(ttyfd, TCSANOW, &ttyinfo);
            else
                /* The entered name didn't match any setting. */
                printf("spl_stty: Invalid argument %s\n", argv[1]);
        }
    }
    else { /* User entered two or more words after spl_stty.                */
        /* Expecting argv[1] to be a variable and argv[2] to be its value.  */
        if ( set_var(&ttyinfo, argv[1], argv[2]) )
            /* Successfully changed value of variable. */
            tcsetattr(ttyfd, TCSANOW, &ttyinfo);
        else { /* Was unable to make the change.       */
            printf("spl_stty: Unable to modify %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}

