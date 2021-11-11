#ifndef TERCON_H
#define TERCON_H

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_GREEN_BG "\x1b[48;5;34m"
#define ANSI_COLOR_RED_BG "\x1b[48;5;196m"
#define ANSI_COLOR_BLUE_BG "\x1b[48;5;21m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_BOLD_FONT "\x1b[1m"
#define ANSI_ITALIC_FONT "\x1b[3m"
#define ANSI_ITALIC_FONT_OFF "\x1b[23m"
#define ANSI_UNDERLINE "\x1b[4m"
#define ANSI_BLINK_SLOW "\x1b[5m"
#define ANSI_BLINK_OFF "\x1b[25m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_CLEAR_SCREEN "\x1b[H\x1b[J"
#define ANSI_MOVE_CURSOR_TO "\x1b[%d;%dH"
#define ANSI_ERASE_LINE "\x1b[2K" // from cursor position and after

typedef struct {
    int rows;
    int columns;
    int cursor_x;
    int cursor_y;
} Terminal;

Terminal tercon_init_rows_cols();
void tercon_clear_screen();
void tercon_clear_error_log();
void tercon_get_cols_rows(int *cols, int *rows);
void tercon_move_y_x(int y, int x);
void tercon_enter_alt_screen();
void tercon_exit_alt_screen();
void tercon_echo_off();
void tercon_echo_on();


#endif /* TERCON_H */

