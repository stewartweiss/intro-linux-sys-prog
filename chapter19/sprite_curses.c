/*****************************************************************************
  Title          : sprite_curses.c
  Author         : Stewart Weiss
  Created on     : July 28, 2024
  Description    : Puts terminal into noncanonical, but not quite raw mode
  Purpose        : To show how to control the terminal .
  Usage          :
  Build with     : gcc -Wall -I../include -L../lib -o sprite_curses \
                      sprite_curses.c  -lspl -lncurses

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
#include <curses.h>
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
sprite sprite_obj;          /* The sprite object                            */
int    direction;           /* The sprite's current direction               */
int    count = 0;           /* Number of times the sprite moved             */


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
    if ( 0 == sp.pos.c  && cur_direction == LEFT )
        return LEFT;
    else if ( COLS-1 == sp.pos.c  && cur_direction == RIGHT )
        return RIGHT;
    else if ( 0 == sp.pos.r && cur_direction == UP )
        return UP;
    else if ( LINES-2 == sp.pos.r && cur_direction == DOWN )
        return DOWN;
    else
        return 0;
}

/* Start sprite_obj in the middle row, left column. */
void init_sprite(sprite *sprite_obj)
{
    move(LINES/2, 0);
    sprite_obj->pos.r = LINES/2;
    sprite_obj->pos.c = 0;
    sprite_obj->symbol = '>';
}

void show_moves( int count)
{
    char   moves[16]; /* Message containing count of positions drawn so far */
    sprintf(moves, "  moves:   %d", count);
    addstr(moves);
}

void show_moves_only(int count)
{
    move(LINES-1, 0);
    show_moves(count);
}


void show_menubar(int count)
{
    mvaddstr(LINES-1, 0, MENU);
    show_moves(count);
}



void setup_screen(int count, sprite *sprite_obj, int *initial_dir)
{
    if ( COLS >= menu_length + 16 )
        show_menubar(count);
    else
        show_moves_only(count);
    init_sprite(sprite_obj);
    *initial_dir = RIGHT;
}



/* If the window is resized while the program is running, don't try to
   save its state. Instead, just restart everything with the new size. */
void on_resize( int signo )
{
    int lines, cols;
    endwin();                 /* End this window and restart.               */
    refresh();                /* Need to refresh to clean up.               */
    initscr();                /* Reinitialize curses.                       */
    clear();                  /* Clear the screen.                          */
    init_sprite(&sprite_obj); /* Reset the sprite to the starting state.    */
    direction = RIGHT;        /* Set it to move to the right.               */
    count  = 0;               /* Reset the count to zero.                   */
    getmaxyx(stdscr, lines, cols);
    if ( cols >= menu_length + 16 )  /* If no room for menu, skip it.       */
        show_menubar(0);
    else {  /* Draw the menu in the new bottom row and show the move count. */
        move(lines-1, 0);
        show_moves(0);
    }
    refresh();
}


void cleanup( int signum )
{
    endwin();
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



int main (int argc, char *argv[])
{
    char   ch;              /* Character entered by user               */
    int    done  = 0;       /* Whether user still wants to run program */
    int    pause = 0;       /* Controls pausing of output              */
    int    delay = USECS;   /* Amount to sleep between moves           */

    unsetenv("LINES");
    unsetenv("COLUMNS");
    setup_sighandlers();
    initscr();
    clear();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);


    /* Clear the screen, put sprite_obj in start position and set direction. */
    setup_screen(count, &sprite_obj, &direction);

    /* Start drawing. */
    while ( !done  ) {
        if ( ! pause ) {
            count++;

            /* If the next move would hit window boundary, move to right
              of current direction. */
            switch ( on_boundary(sprite_obj, LINES, COLS, direction)) {
                case UP   : direction = RIGHT; break;
                case RIGHT: direction = DOWN; break;
                case DOWN : direction = LEFT; break;
                case LEFT : direction = UP; break;
                default   : /* No change */
            }
            /* Draw sprite in next position */
            move(sprite_obj.pos.r, sprite_obj.pos.c);
            addch(sprite_obj.symbol);
            update_sprite(&sprite_obj, direction);
        }
        if ( COLS >= menu_length + 16 )
            show_menubar(count);
        else
            show_moves_only(count);
        refresh();
        usleep(delay);        /* delay a bit */

        if ( ERR != (ch = getch() )) {
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
    endwin();
    return 0;
}





