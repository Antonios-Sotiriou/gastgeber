/********************
* Build in libraries
*********************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>
/*********************
 * Global constants 
 ********************/

/*********************
 * Color Initialisation
 ********************/
#include "header_files/tercon.h"
/*******************************************************
 * My own libraries, collection of functions and structs
 ******************************************************/

Terminal tercon_init_rows_cols() {

    Terminal term;
    //signal(SIGWINCH, &tercon_win_size_changed);

    int rows = 0;
    int cols = 0;

    tercon_get_cols_rows(&cols, &rows);
    //tercon_move_y_x(0, (cols - 53) / 2);

    term.rows = rows;
    term.columns = (cols - 53) / 2;

    return term;
}

void tercon_clear_screen()
{
    printf("\x1b[H\x1b[J");
}

void tercon_get_cols_rows(int *cols, int *rows)
{

    struct winsize size;
    ioctl(1, TIOCGWINSZ, &size);

    *rows = size.ws_row;
    *cols = size.ws_col;
}

void tercon_move_y_x(int y, int x)
{

    printf("\x1b[%d;%dH", y, x);
}

void tercon_win_size_changed()
{

    int rows = 0;
    int cols = 0;

    tercon_get_cols_rows(&cols, &rows);

    printf("\x1b[H\x1b[J");
    tercon_move_y_x(rows / 2, cols / 2);
}

void tercon_enter_alt_screen()
{

    printf("\x1b[?1049h");
}

void tercon_exit_alt_screen()
{

    printf("\x1b[?1049l");
}

void tercon_echo_off()
{

    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(1, TCSANOW, &term);
}

void tercon_echo_on()
{

    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag |= ECHO;
    tcsetattr(1, TCSANOW, &term);
}
