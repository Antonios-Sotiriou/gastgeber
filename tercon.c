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

/**********************************************
 * Color Initialisation and Terminal management 
 **********************************************/
#include "header_files/tercon.h"
/*******************************************************
 * My own libraries, collection of functions and structs
 ******************************************************/

Terminal tercon_init_rows_cols() {

    Terminal term;

    int rows = 0;
    int cols = 0;

    tercon_get_cols_rows(&cols, &rows);

    term.rows = rows;
    term.columns = cols;

    return term;
}

void tercon_clear_screen() {

    printf("\x1b[H\x1b[J");
}

void tercon_clear_error_log() {

    Terminal term = tercon_init_rows_cols();

    printf("\x1b[%d;%dH                                                                                ", term.rows -2, (term.columns - 80) / 2);
    printf("\x1b[%d;%dH                                                                                ", term.rows -1, (term.columns - 80) / 2);
    printf("\x1b[%d;%dH                                                                                ", term.rows, (term.columns - 80) / 2);
}

void tercon_get_cols_rows(int *cols, int *rows) {

    struct winsize size;
    ioctl(1, TIOCGWINSZ, &size);

    *rows = size.ws_row;
    *cols = size.ws_col;
}

void tercon_move_y_x(int y, int x) {

    printf("\x1b[%d;%dH", y, x);
}

void tercon_enter_alt_screen() {

    printf("\x1b[?1049h");
}

void tercon_exit_alt_screen() {

    printf("\x1b[?1049l");
}

void tercon_echo_off() {

    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(1, TCSANOW, &term);
}

void tercon_echo_on() {

    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag |= ECHO;
    tcsetattr(1, TCSANOW, &term);
}

void tercon_en_raw() {

    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag &= ~ICANON;
    tcsetattr(1, TCSANOW, &term);
}

void tercon_dis_raw() {

    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag |= ICANON;
    tcsetattr(1, TCSANOW, &term);
}

void tercon_hide_cursor() {
    printf("\x1b[?25l");
}

void tercon_show_cursor() {
    printf("\x1b[?25h");
}

