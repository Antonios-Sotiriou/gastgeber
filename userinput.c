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
#include "header_files/global/global_vars.h"

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
/* Accepts Date input in form dd/mm/yyyy and doesn't consume \n */
int getDate(int max_len, char *input) {

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
                input[i] = c;
                i++;
            // OTHERWISE SPECIAL CHARACTER BUT ALLOW SPACES
            } else if (c == 47) {
                input[i] = c;
                i++;
            } else {
                return -2;
            }
        } else {
            // Given input exceeds wanted length
            return -3;
        }
    }
    input[i] = '\0';

    return 0;
}
/* Gets a float of length [ max_len ].
    In length counts also the [ . ] float separator. */
float getfloat(int max_len) {

    char cleared_num[max_len + 1];
    int buffer_overflow = 0;
    int i = 0;
    int dot_count = 0;

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
            // OTHERWISE SPECIAL CHARACTER BUT ALLOW A DOT
            } else if (c == '.' && dot_count == 0) {
                cleared_num[i] = c;
                i++;
                dot_count++;
            } else {
                return -2;
            }
        } else {
            // Given input exceeds wanted length
            return -3;
        }
    }
    cleared_num[i] = '\0';

    return strtof(cleared_num, NULL);
}
/* Accepts date input in form dd/mm/yyyy */
int getformatedDate(char *room_date) { 

    char input[40];
    int day;
    int month;
    int year;
    time_t current_time;
    char converted_day[4];
    char converted_month[4];
    char converted_year[5];
    
    Terminal term = tercon_init_rows_cols();

    time(&current_time);

    struct tm *mytime = localtime(&current_time);
    // for debugging purposes...
    //printf("%i/%i/%i\n", mytime -> tm_mday, mytime -> tm_mon + 1, mytime -> tm_year + 1900);

    int date_test = getDate(11, input);
    if (date_test == 0) {
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
    } else {
        tercon_clear_error_log();
        printf(ANSI_COLOR_RED "\x1b[%d;%dHThe date you entered is in wrong format!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2);
        return date_test;
    }
    return 1;
}
/* Accepts date input in form dd/mm/yyyy.Accepts also dates in the past until Timedelta 1900*/
int getTimelessDate(char *room_date) {

    char input[40];
    int day;
    int month;
    int year;
    char converted_day[4];
    char converted_month[4];
    char converted_year[5];

    Terminal term = tercon_init_rows_cols();

    int date_test = getDate(11, input);
    if (date_test == 0) {

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
            if(year < STARTING_YEAR) {
                tercon_clear_error_log();
                printf(ANSI_COLOR_RED "\x1b[%d;%dHNo record for this year!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 24) / 2);
                return 0;
            } else {
                sprintf(converted_year, "%d", year);
            }
        } else {
            tercon_clear_error_log();
            printf(ANSI_COLOR_RED "\x1b[%d;%dHThe date you entered is in wrong format!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2);
            return 0;
        }
        sprintf(room_date, "%s%s%s", converted_day, converted_month, converted_year);
    } else {
        tercon_clear_error_log();
        printf(ANSI_COLOR_RED "\x1b[%d;%dHThe date you entered is in wrong format!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2);
        return date_test;
    }
    return 1;
}
/* Accepts a string of length [str_len] without spaces at start, end or between. */
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
/* Accepts a string of length [str_len] and allow spaces. */
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

