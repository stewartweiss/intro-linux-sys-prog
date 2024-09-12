/*****************************************************************************
  Title          : sprite.c
  Author         : Stewart Weiss
  Created on     : July 28, 2024
  Description    : Puts terminal into noncanonical, but not quite raw mode
  Purpose        : To show how to control the terminal .
  Usage          : sprite_demo
  Build with     : gcc -Wall -I../include -L../lib -o sprite \
                      sprite.c  -lspl
******************************************************************************
* Copyright (C) 2024 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#include "common_hdrs.h"
#include <termios.h>
#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif

/* Directions of movement */
#define UP     1
#define RIGHT  2
#define DOWN   3
#define LEFT   4

#define USECS  400000    /* Default amount of time to sleep between updates */

const char MENU[]         = "quit:q; pause:p; continue:c; up:u; "
                             "right:r; down:d; left:l ";
const int  menu_length    = strlen(MENU);

/* ANSI Escape sequences */
const char CURSOR_HOME[]  = "\033[1;1H";
const char CLEAR_SCREEN[] = "\033[2J";
const char CLEAR_LINE[]   =  "\033[1A\033[2K\033[G";
const char HIDE_CURSOR[]  = "\033[?25l";
const char SHOW_CURSOR[]  = "\033[?25h";
const char USE_ALTSCREEN[] = "\e[?1049h";
const char USE_OLDSCREEN[] = "\e[?1049l";

/* Screen coordinate position */
typedef struct {
    int r;
    int c;
} screenpos;


/* A sprite representation, consisting of a position and a glyph to draw. */
typedef struct {
    screenpos pos;
    char      symbol;
} sprite;

/* The four possible unit directions of motion. By adding these to a
   position, it advances in that direction. */
const screenpos Right  = {0,1};
const screenpos Left  = {0,-1};
const screenpos Up = {-1,0};
const screenpos Down = {1.0};

/* The sprite_state array simplifies updating the sprite when it changes
direction. */
sprite  sprite_state[] = {
        { {0,0},  ' '},
        { Up,    '^' },
        { Right, '>' },
        { Down,  'v' },
        { Left,  '<' }
        };


/* Global variables */
struct termios savedtty;    /* Initial state of terminal; restored on exit  */
int    numrows;             /* Current number of rows in terminal screen    */
int    numcols;             /* Current number of columns in terminal screen */
sprite sprite_obj;              /* The sprite object                            */
int    direction;           /* The sprite's current direction               */
int    count = 0;           /* Number of times the sprite moved             */


void addto(screenpos *target, const screenpos adjust);
void update_sprite(sprite *sp, int dir);
int  on_boundary(sprite sp, int rows, int cols, int cur_direction);
void init_sprite(sprite *sprite_obj);

void moveto(int line, int col );
int  get_window_size( int ttyfd, int *rows, int *cols );
void on_resize( int signo );
void clear_screen();

int  init_terminal(int ttyfd );
void save_tty();
void restore_tty();

void cleanup( int signum );
void setup_sighandlers();
void show_moves(int count);
void show_moves_only(int count);
void show_menubar(int count);
void setup_screen(int count, sprite *sprite_obj, int *initial_dir);


void addto(screenpos *target, const screenpos adjust)
{
    target->r += adjust.r;
    target->c += adjust.c;
}

/* update sprite(s, d) changes character displayed and direction of motion. */
void update_sprite(sprite *sp, int dir)
{
    addto(&(sp->pos), sprite_state[dir].pos);
    sp->symbol = sprite_state[dir].symbol;
}


int on_boundary(sprite sp, int rows, int cols, int cur_direction)
{
    if ( 1 == sp.pos.c  && cur_direction == LEFT )
        return LEFT;
    else if ( cols == sp.pos.c  && cur_direction == RIGHT )
        return RIGHT;
    else if ( 1 == sp.pos.r && cur_direction == UP )
        return UP;
    else if ( rows -1 == sp.pos.r && cur_direction == DOWN )
        return DOWN;
    else
        return 0;
}

/* Start sprite_obj in the middle row, left column. */
void init_sprite(sprite *sprite_obj)
{
    moveto(numrows/2, 1);
    sprite_obj->pos.r = numrows/2;
    sprite_obj->pos.c = 1;
    sprite_obj->symbol = '>';
}


void moveto(int line, int col )
{
    char seq_str[20];

    sprintf(seq_str, "\033[%d;%dH", line, col);
    write(STDOUT_FILENO, seq_str, strlen(seq_str));
}

void enter_alt_screen(void)
{
    write(STDOUT_FILENO, USE_ALTSCREEN,strlen(USE_ALTSCREEN) );
}
void leave_alt_screen(void)
{
    write(STDOUT_FILENO, USE_OLDSCREEN,strlen(USE_OLDSCREEN) );
}



void clear_screen()
{
    write(STDOUT_FILENO, CLEAR_SCREEN, strlen(CLEAR_SCREEN));
}


/* If the window is resized while the program is running, don't try to
   save its state. Instead, just restart everything with the new size. */
void on_resize( int signo )
{
    struct winsize size;

    if (ioctl(1, TIOCGWINSZ,  &size) < 0)
        fatal_error(errno, "TIOCGWINSZ error");
    numrows = size.ws_row;    /* Store new size.   */
    numcols = size.ws_col;
    clear_screen();           /* Clear the screen. */
    init_sprite(&sprite_obj); /* Reset the sprite to the starting state.    */
    direction = RIGHT;        /* Set it to move to the right.               */
    count  = 0;               /* Reset the count to zero.                   */
    if ( numcols >= menu_length + 16 )  /* If no room for menu, skip it.    */
        show_menubar(0);
    else {  /* Draw the menu in the new bottom row and show the move count. */
        moveto(numrows, 1);
        write(STDOUT_FILENO, CLEAR_LINE, strlen(CLEAR_LINE));
        moveto(numrows, 1);
        show_moves(0);
    }
}

/****************************************************************************/

int  get_window_size( int ttyfd, int *rows, int *cols )
{
    struct winsize size;
    int retval;

    if ( -1 != ( retval = ioctl(ttyfd, TIOCGWINSZ,  &size) ) ) {
        *rows = size.ws_row;
        *cols = size.ws_col;
    }
    return retval;
}

/****************************************************************************/
int init_terminal(int ttyfd )
{
    struct termios cur_tty;

    if ( -1 == tcgetattr(ttyfd, &cur_tty) )
        return ( -1 );
    cur_tty.c_lflag     &= ~ICANON;
    cur_tty.c_lflag     &= ~ECHO;
    cur_tty.c_lflag     &= ~ISIG;
    cur_tty.c_cc[VMIN]   =  0;
    cur_tty.c_cc[VTIME]  =  0;
    return ( tcsetattr(ttyfd, TCSANOW, &cur_tty) );
}

/*****************************************************************************/
void save_tty()
{
    if ( -1 == tcgetattr(STDIN_FILENO, &savedtty) )
        fatal_error(errno, "tcgetattr");
}

void restore_tty()
{
    if ( -1 == tcsetattr(STDIN_FILENO,TCSANOW, &savedtty) )
        fatal_error(errno, "tcsetattr");
}

/****************************************************************************/


void cleanup( int signum )
{
    write(STDOUT_FILENO, SHOW_CURSOR, strlen(SHOW_CURSOR));
    clear_screen();
    restore_tty();
    leave_alt_screen();
    moveto(numrows, 1);
    raise(SIGTERM);
}

void setup_sighandlers()
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = on_resize;
    sigaction(SIGWINCH, &sa, NULL);

    sa.sa_handler = cleanup;
    if ( sigaction(SIGINT, &sa, NULL) == -1 )
        fatal_error(errno, "sigaction");
    if ( sigaction(SIGQUIT, &sa, NULL) == -1 )
        fatal_error(errno, "sigaction");
}

void show_moves( int count)
{
    char   moves[16]; /* Message containing count of positions drawn so far */
    sprintf(moves, "  moves:   %d", count);
    write(STDOUT_FILENO, moves, strlen(moves));
}

void show_moves_only(int count)
{
    moveto(numrows, 1);
    show_moves(count);
}


void show_menubar(int count)
{
    moveto(numrows, 1);
    write(STDOUT_FILENO, MENU, strlen(MENU) );
    show_moves(count);
}

void setup_screen(int count, sprite *sprite_obj, int *initial_dir)
{
    clear_screen();
    write(STDOUT_FILENO, HIDE_CURSOR, strlen(HIDE_CURSOR));
    if ( numcols >= menu_length + 16 )
        show_menubar(count);
    else
        show_moves_only(count);
    init_sprite(sprite_obj);
    *initial_dir = RIGHT;
}



int main (int argc, char *argv[])
{
    char   ch;              /* Character entered by user               */
    int    done  = 0;       /* Whether user still wants to run program */
    int    pause = 0;       /* Controls pausing of output              */
    int    delay = USECS;   /* Amount to sleep between moves           */
    setup_sighandlers();

    /* Check whether input or output has been redirected */
    if ( !isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO) )
        fatal_error(-1, "Not a tty");

    /* Save the original tty state */
    save_tty();
    atexit(restore_tty);
    enter_alt_screen();

    /* Modify the terminal - turn off echo, keybd sigs, and canonical mode.
       Also set MIN = 0 and TIME = 0 to put in polling mode.                */
    if ( -1 == init_terminal( STDIN_FILENO) )
        fatal_error(-1, "Could not configure the terminal.");

    /* Get the window's size */
    if ( -1 == get_window_size(STDIN_FILENO, &numrows, &numcols))
        fatal_error(errno, "TIOCGWINSZ error");

    /* Clear the screen, put sprite_obj in start position and set direction. */
    setup_screen(count, &sprite_obj, &direction);

    /* Start drawing. */
    while ( !done  ) {
        if ( ! pause ) {
            count++;

            /* If the next move would hit window boundary, move to right
              of current direction. */
            switch ( on_boundary(sprite_obj, numrows, numcols, direction)) {
                case UP   : direction = RIGHT; break;
                case RIGHT: direction = DOWN; break;
                case DOWN : direction = LEFT; break;
                case LEFT : direction = UP; break;
                default   : /* No change */
            }
            /* Draw sprite in next position */
            moveto(sprite_obj.pos.r, sprite_obj.pos.c);
            write(STDOUT_FILENO, &(sprite_obj.symbol), 1);
            update_sprite(&sprite_obj, direction);
        }
        if ( numcols >= menu_length + 16 )
            show_menubar(count);
        else
            show_moves_only(count);
        usleep(delay);        /* delay a bit */
        /* Do the read. If nothing was typed, do nothing */
        if ( read(STDIN_FILENO, &ch, 1)  > 0 ) {
            switch( ch ) {
            case 'q':
                done = 1;  break;
            case 'p':
                pause = 1; break;
            case 'c' :
                pause = 0; break;
            case 'u':
                direction = UP; break;
            case 'd':
                direction = DOWN; break;
            case 'l':
                direction = LEFT; break;
            case 'r':
                direction = RIGHT; break;
            }
        }
    }
    /* Cleanup -- flush queue, clear the screen, and restore terminal */
    tcflush(STDIN_FILENO, TCIFLUSH);
    cleanup(0);
    return 0;
}





