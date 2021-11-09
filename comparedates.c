/********************
* Build in libraries
*********************/
#include <stdio.h>
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
#include "header_files/comparedates.h"

int compareDates(char *from_date, char *to_date) {

    int from_day;
    int from_month;
    int from_year;

    int to_day;
    int to_month;
    int to_year;

    Terminal term = tercon_init_rows_cols();

    sscanf(from_date, "%d/%d/%d", &from_day, &from_month, &from_year);
    sscanf(to_date, "%d/%d/%d", &to_day, &to_month, &to_year);

    if(to_year < from_year) {
        return 0;
    } else if(to_year ==  from_year && to_month > from_month) {
        return 1;
    } else if(to_year == from_year && to_month == from_month) {
        if(to_day > from_day) {
            return 1;
        } else if(to_day == from_day) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dH           Same from and to days.You must provide a day in the future.          \n" ANSI_COLOR_RESET, term.rows - 2, (term.columns - 80) / 2);
            printf(ANSI_COLOR_RED "\x1b[%d;%dH                 You can't check in and check out the same day!                 \n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 80) / 2);
            return 0;
        } else {
            printf(ANSI_COLOR_RED "\x1b[%d;%dH                            This day is in the past!                            \n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 80) / 2);
            return 0;         
        }
    } else if(to_year > from_year) {
        return 1;
    } else {
        printf(ANSI_COLOR_RED "\x1b[%d;%dH                        To date is older than from date!                            \n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 80) / 2);
        return 0;
    }
}

