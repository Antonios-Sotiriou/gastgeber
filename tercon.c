/********************
* Build in libraries
*********************/
#include <stdio.h>
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

/* Returns a terminal struct with rows and columns number. */
Terminal tercon_init_rows_cols() {

    Terminal term;

    int rows = 0;
    int cols = 0;

    tercon_get_cols_rows(&cols, &rows);

    term.rows = rows;
    term.columns = cols;

    return term;
}
/* Clears screen and returns cursor at 0 rows 0 columns position. */
void tercon_clear_screen() {

    printf("\x1b[H\x1b[J");
}
/* Clears the error log by printing empty lines. */
void tercon_clear_error_log() {

    Terminal term = tercon_init_rows_cols();

    printf("\x1b[%d;%dH                                                                                ", term.rows -2, (term.columns - 80) / 2);
    printf("\x1b[%d;%dH                                                                                ", term.rows -1, (term.columns - 80) / 2);
    printf("\x1b[%d;%dH                                                                                ", term.rows, (term.columns - 80) / 2);
}
/* Clears lines [ from ] [ to ] .If from > to clears decremental else incremental. */
void tercon_clear_lines(int from, int to) {
    /**********************************************************************************  
        Clear lines of terminal.If [from] is greater than [to] clear from bottom to top
        else clear from top to bottom
    **********************************************************************************/
    if (from > to) {
        for (int i = from; i >= to; i--) {
            printf("\x1b[%d;0H\x1b[2K", i);
        }
    } else if (from < to) {
        for (int i = from; i <= to; i++) {
            printf("\x1b[%d;0H\x1b[2K", i);
        }
    }
}
/* Aquiring the terminal window size in columns and rows. */
void tercon_get_cols_rows(int *cols, int *rows) {

    struct winsize size;
    ioctl(1, TIOCGWINSZ, &size);

    *rows = size.ws_row;
    *cols = size.ws_col;
}
/* Positioning the cursor at y row, x column. */
void tercon_move_y_x(int y, int x) {

    printf("\x1b[%d;%dH", y, x);
}
/* Enables the screen alternative buffer. */
void tercon_enter_alt_screen() {

    printf("\x1b[?1049h");
}
/* Disables the screen alternative buffer. */
void tercon_exit_alt_screen() {

    printf("\x1b[?1049l");
}
/* Disables the input echoing. */
void tercon_echo_off() {

    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(1, TCSANOW, &term);
}
/* Enables the input echoing. */
void tercon_echo_on() {

    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag |= ECHO;
    tcsetattr(1, TCSANOW, &term);
}
/* Enables terminal raw input mode. */
void tercon_en_raw() {

    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag &= ~ICANON;
    tcsetattr(1, TCSANOW, &term);
}
/* Disables terminal raw input mode. */
void tercon_dis_raw() {

    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag |= ICANON;
    tcsetattr(1, TCSANOW, &term);
}
/* Disables cursor show. */
void tercon_hide_cursor() {

    printf("\x1b[?25l");
}
/* Enables cursor show. */
void tercon_show_cursor() {
    
    printf("\x1b[?25h");
}

