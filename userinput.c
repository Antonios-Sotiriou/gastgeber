/********************
* Build in libraries
*********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
/**********************************************
 * Color Initialisation and Terminal management 
 **********************************************/
#include "header_files/tercon.h"
/*******************************************************
 * My own libraries, collection of functions and structs
 ******************************************************/
#include "header_files/userinput.h"

/* Clears buffer until it meets a New Line.It doesn't consume the New Line char. */
void buffer_clear() {
    char c;
    while((c = getc(stdin) != '\n') && c != '\t');
}
/* Get an Unsigned Integer of length [ max_len ].
   If [ spaces = true ], allow spaces at start, middle or between.
   Returns the Integer without spaces. */
int getnuminput(int max_len, bool spaces) {

    char cleared_num[max_len + 1];
    int buffer_overflow = 0;
    int i = 0;

    while (!buffer_overflow) {
        char c = getc(stdin);
        if (c == '\n' || c == EOF) {
            buffer_overflow = 1;
        } else if (i <= max_len) {
        // CHECKING FOR ALPHABET
            if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
                return -1;
            // CHECKING FOR DIGITS
            } else if (c >= 48 && c <= 57) {
                cleared_num[i] = c;
                i++;
            // OTHERWISE SPECIAL CHARACTER BUT ALLOW SPACES
            } else if (spaces) {
                if (c != ' ') {
                    return -2;
                }
            } else {
                return -2;
            }
        } else {
            // Given input exceeds wanted length
            return -3;
        }
    }
    cleared_num[i] = '\0';

    return atoi(cleared_num);
}
int getformatedDate(char *room_date) { 

    char input[40];
    int day;
    int month;
    int year;
    time_t current_time;
    char converted_day[20];
    char converted_month[20];
    char converted_year[20];
    
    Terminal term = tercon_init_rows_cols();

    time(&current_time);

    struct tm *mytime = localtime(&current_time);
    // for debugging purposes...
    //printf("%i/%i/%i\n", mytime -> tm_mday, mytime -> tm_mon + 1, mytime -> tm_year + 1900);

    fgets(input, 40, stdin);
    int test = sscanf(input, "%d/%d/%d", &day, &month, &year);

    if(test == 3) {
        if(day < 0 || day > 31) {
            tercon_clear_error_log();
            printf(ANSI_COLOR_RED "\x1b[%d;%dHGive a correct day!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 19) / 2);
            return 0;
        } else if(day > 0 && day < 10) {
            sprintf(converted_day, "0%d/", day);
        } else if(day > 9 && day <= 31) {
            sprintf(converted_day, "%d/", day);
        }
        if(month < 0 || month > 12) {
            tercon_clear_error_log();
            printf(ANSI_COLOR_RED "\x1b[%d;%dHGive a correct month!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 21) / 2);
            return 0;
        } else if(month > 0 && month < 10) {
            sprintf(converted_month, "0%d/", month);
        } else if(month > 9) {
            sprintf(converted_month, "%d/", month);
        }
        if(year > mytime -> tm_year + 1900) {
            sprintf(converted_year, "%d", year);
        } else if(year == mytime -> tm_year + 1900 && month - (mytime -> tm_mon + 1) > 0) {
            sprintf(converted_year, "%d", year);
        } else if(year == mytime -> tm_year + 1900 && month - (mytime -> tm_mon + 1) == 0) {
            if(day - mytime -> tm_mday >= 0) {
                sprintf(converted_year, "%d", year);
            } else {
                tercon_clear_error_log();
                printf(ANSI_COLOR_RED "\x1b[%d;%dHThis day is in the past!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 24) / 2);
                return 0;
            }
        } else {
            tercon_clear_error_log();
            printf(ANSI_COLOR_RED "\x1b[%d;%dHProvide a date from today to the future!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2);
            return 0;
        }
    } else {
        tercon_clear_error_log();
        printf(ANSI_COLOR_RED "\x1b[%d;%dHThe date you entered is in wrong format!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2);
        return 0;
    }
    sprintf(room_date, "%s%s%s", converted_day, converted_month, converted_year);
    return 1;
}
/* Get integer */
int getInteger(int str_len, int int_len) {

    /**************************************
     * Return a number of the provided int_len.To avoid input overflow give
     * always a str_len a little bigger than int_len so we can also check if
     * user input exceeds the required int_len.
     * ************************************/
    
    int user_input = 0;
    char str_input[str_len + 1];

    fgets(str_input, sizeof(str_input), stdin);

    if(strlen(str_input) - 1 >= int_len) {
        // Invalid number length.
        return -1;
    } else {
        for(int i = 0; i <= strlen(str_input) - 1; i++) {
            if(str_input[i] >= 48 && str_input[i] <= 57) {
                user_input = user_input * 10 + str_input[i] - '0';
            } else if (str_input[i] != '\n') {
                // Wrong syntax.User provided also spaces or other symbols.
                return -2;
            }
        }
    }
    return user_input;  
}
char* getString(int str_len) {

    /**********************************************************************
     * Return a string of given length without spaces in between
     **********************************************************************/

    char str_input[48];
    char *cleaned_str = malloc(sizeof(char));

    Terminal term = tercon_init_rows_cols();

    fgets(str_input, sizeof(str_input), stdin);
    int i;
    int dynamic_inc = 1;
    // strlen(str_input) - 2 here to exclude \n and \0 chars that appended at the end.Not sure if thats needed because anyway we count until -2 str elements. 
    for(i = 0; i <= strlen(str_input) - 2; i++) {
        if(str_input[i] == ' ' || str_input[i] == '\t') {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHSpaces are not allowed between.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 31) / 2);
            return 0;
        } else if(str_input[i] == 0) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHNo input Provided.getString().\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 30) / 2);
            return 0;
        } else {
            cleaned_str = realloc(cleaned_str, sizeof(char) * dynamic_inc);
            cleaned_str[i] = str_input[i];
            dynamic_inc++;
        }
    } 
    cleaned_str[i] = '\0';
    // Here checking the string size + 1 to give an extra place for the \0 char.
    if(strlen(cleaned_str) > str_len) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid length.getString()." ANSI_COLOR_RESET, term.rows - 1, (term.columns - 26) / 2);
        printf(ANSI_COLOR_RED "\x1b[%d;%dHMust be maximum %d characters long." ANSI_COLOR_RESET, term.rows, (term.columns - 35) / 2, str_len);
        return 0;
    } else {
        return cleaned_str;
    }
}
char* getSpString(int str_len) {

    /**********************************************************************
     * Return a string of given length with spaces in between
     **********************************************************************/

    char str_input[48];
    char *cleaned_str = malloc(sizeof(char));

    Terminal term = tercon_init_rows_cols();

    fgets(str_input, sizeof(str_input), stdin);
    int i;
    int dynamic_inc = 1;
    // strlen(str_input) - 2 here to exclude \n and \0 chars that appended at the end.
    for(i = 0; i <= strlen(str_input) - 2; i++) {
        if(str_input[i] == 0) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHNo input Provided.getSpString().\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 32) / 2);
            return 0;
        } else {
            cleaned_str = realloc(cleaned_str, sizeof(char) * dynamic_inc);
            cleaned_str[i] = str_input[i];
            dynamic_inc++;
        }
    } 
    cleaned_str[i] = '\0';
    // Here checking the string size + 1 to give an extra place for the \0 char.
    if(strlen(cleaned_str) > str_len) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid length.getSpString()\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 28) / 2);
        printf(ANSI_COLOR_RED "\x1b[%d;%dHMust be maximum %d characters long." ANSI_COLOR_RESET, term.rows, (term.columns - 35) / 2, str_len);
        return 0;
    } else {
        return cleaned_str;
    }
}
/*
int getnuminput(int max_len) {

    char input[48];
    char cleared_num[48];

    fgets(input, sizeof(input), stdin);

    if(strlen(input) - 1 > max_len) {
        printf(ANSI_COLOR_RED "\nInvalid number length!\n" ANSI_COLOR_RESET);
        return 0;
    }
    for(int i = 0, d = 0; i <= strlen(input) - 1; i++) {
        // CHECKING FOR ALPHABET
        if ((input[i] >= 65 && input[i] <= 90) || (input[i] >= 97 && input[i] <= 122)) {
            return 0;
        // CHECKING FOR DIGITS
        } else if (input[i] >= 48 && input[i] <= 57) {
            cleared_num[d] = input[i];
            d++;
        // OTHERWISE SPECIAL CHARACTER
        } else {
            continue;
        }
    }
    return atoi(cleared_num);
}
*/

