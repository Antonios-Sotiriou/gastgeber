/********************
* Build in libraries
*********************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
/*********************
 * Global constants 
 ********************/
#include "header_files/global/global_vars.h"
#include "header_files/global/days_db_path.h"
#include "header_files/global/journal_main_path.h"
/**********************************************
 * Color Initialisation and Terminal management 
 **********************************************/
#include "header_files/tercon.h"
/*******************************************************
 * My own libraries, collection of functions and structs
 ******************************************************/
#include "structures/room.h"
#include "structures/guest.h"
#include "structures/day.h"
#include "structures/reservations.h"

#include "header_files/reserve.h"
#include "header_files/display.h"
#include "header_files/joins.h"
/* Checks if Room is reserved for res.from_date.
    Checks also if res.from_date is valid. */
int checkFromDate(struct Reservation res) {

    struct Day day;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, daysdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate daysdb file checkFromDate()");
        exit(127);
    }

    Terminal term = tercon_init_rows_cols();

    int starting_point;
    int found = 0;
    // While loop to find the startind day;
    while(found == 0) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else if(strcmp(day.date, res.from_date) == 0) {
            starting_point = day.id;
            found = 1;
            for(int i = 1; i <= sizeof(day.room_id) / sizeof(int); i++) {
                // Check if the room id is already in the days rooms ids.
                if(day.room_id[i] == res.room.id) {
                    tercon_clear_error_log();
                    printf(ANSI_COLOR_RED "\x1b[%d;%dHRoom is already reserved for this date: %s\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 50) / 2, day.date);
                    fclose(fp);
                    return 1;
                }
            }
        } else {
            starting_point = 0;
        }
    }
    fclose(fp);
    
    if(starting_point <= 0) {
        tercon_clear_error_log();
        printf(ANSI_COLOR_RED "\x1b[%d;%dHCheck the dates please.Be sure that this day exists in this year\n" ANSI_COLOR_RESET, term.rows - 2, (term.columns - 64) / 2);
        printf(ANSI_COLOR_RED "\x1b[%d;%dHFor example 29 of February or 31 of November.Error: checkFromDate()!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 68) / 2);
        return 1;
    }
    return 0;
}
/* Checks if room is reserved for any of the days between res.from_date and res.to_date.
    Also checks if dates are valid. */
int checkAllDates(struct Reservation res) {

    struct Day day;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, daysdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate daysdb file checkFromDate()");
        exit(127);
    }

    Terminal term = tercon_init_rows_cols();

    int starting_point;
    int finishing_point;
    // While loop to find the startind and ending days ids;
    int found = 0;
    while(found < 2) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else if(strcmp(day.date, res.from_date) == 0) {
            starting_point = day.id;
            found += 1;
        } else if(strcmp(day.date, res.to_date) == 0) {
            finishing_point = day.id;
            found += 1;
        } else {
            finishing_point = 0;
        }
    }

    if (finishing_point <= 0) {
        tercon_clear_error_log();
        printf(ANSI_COLOR_RED "\x1b[%d;%dHCheck the dates please.Be sure that this day exists in this year.\n" ANSI_COLOR_RESET, term.rows - 2, (term.columns - 64) / 2);
        printf(ANSI_COLOR_RED "\x1b[%d;%dHFor example 29 of February or 31 of November.Error: checkAllDates()!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 68) / 2);
        fclose(fp);
        return -1;
    }
    // Go to the beggining of the file and readall data again
    int num_of_days = 0;
    fseek(fp, 0, SEEK_SET);
    while(1) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else {
            // Get the days from starting day id to end day id
            if(day.id >= starting_point && day.id <= finishing_point) {

                for(int i = 1; i <= sizeof(day.room_id) / sizeof(int); i++) {
                    // Check if the room id is already in the days rooms ids.
                    if(day.room_id[i] == res.room.id) {
                        printf("\x1b[%d;%dH|%s              Room is already reserved for those dates: %s              %s|\n", (term.rows - 2) + num_of_days, (term.columns - 82) / 2, ANSI_COLOR_RED, day.date, ANSI_COLOR_RESET);
                        num_of_days++;
                    }
                }
            }
        }
    }
    fclose(fp);

    if(num_of_days != 0) {
        return num_of_days;
    } else {
        printf(ANSI_COLOR_GREEN "\x1b[%d;%dHShall the reservation be applied: [Y/N]? " ANSI_COLOR_RESET, term.rows - 4, (term.columns - 41) / 2);
        char c = getc(stdin);
        if(c == 'Y' || c == 'y') {
            applyReservation(starting_point, finishing_point, res.room.id);
            addReservationToDates(starting_point, finishing_point, res.id);
            tercon_clear_error_log();
            printf(ANSI_COLOR_GREEN "\x1b[%d;%dHReservation applied success.\n" ANSI_COLOR_RESET, term.rows - 2, (term.columns - 28) / 2);
            getc(stdin);
            return 0;
        } else if (c == '\n' || c == '\t') {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHReservation Canceled!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 21) / 2);
            return -2;
        } else {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHReservation Canceled!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 21) / 2);
            getc(stdin);
            return -2;
        }
    }
}
/* Applies the reservation by adding res_room_id in dates Database from [ start ] day until [ finish ] day. */
void applyReservation(int start, int finish, int res_room_id) {

    struct Day day;
    FILE *fp, *fp1;
    char abs_path[PATH_LENGTH];
    char journal_path[PATH_LENGTH];
    joinHome(abs_path, daysdb);
    joinHome(journal_path, journal_main);
    fp = fopen(abs_path, "rb");
    fp1 = fopen(journal_path, "wb");
    if (fp == NULL) {
        perror("Could not locate daysdb file checkFromDate()");
        fclose(fp1);
        remove(journal_path);
        exit(127);
    }

    while(1) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else {
            // Get the days from starting day id to end day id
            if(day.id >= start && day.id <= finish) {
                day.room_id[res_room_id] = res_room_id;
            }
            fwrite(&day, sizeof(day), 1, fp1);
        }
    }
    fclose(fp);
    fclose(fp1);

    fp = fopen(abs_path, "wb");
    fp1 = fopen(journal_path, "rb");

    while(1) {
        fread(&day, sizeof(day), 1, fp1);
        if(feof(fp1)) {
            break;
        }
        fwrite(&day, sizeof(day), 1, fp);
    }
    fclose(fp1);
    fclose(fp);

    remove(journal_path);
}

/* Applies the reservation by adding res_id in dates Database from [ start ] day until [ finish ] day. */
void addReservationToDates(int start, int finish, int res_id) {

    struct Day day;
    FILE *fp, *fp1;
    char abs_path[PATH_LENGTH];
    char journal_path[PATH_LENGTH];
    joinHome(abs_path, daysdb);
    joinHome(journal_path, journal_main);
    fp = fopen(abs_path, "rb");
    fp1 = fopen(journal_path, "wb");
    if (fp == NULL) {
        perror("Could not locate daysdb file checkFromDate()");
        fclose(fp1);
        remove(journal_path);
        exit(127);
    }

    while(1) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else {
            // Get the days from starting day id to end day id
            if (day.id >= start && day.id <= finish) {
                for (int i = 1; i <= (sizeof(day.res_ids) / sizeof(int)) - 1; i++) {
                    if (day.res_ids[i] >= 1000000) {
                        day.res_ids[i] = res_id;
                        break;
                    }
                }
            }
            fwrite(&day, sizeof(day), 1, fp1);
        }
    }
    fclose(fp);
    fclose(fp1);

    fp = fopen(abs_path, "wb");
    fp1 = fopen(journal_path, "rb");

    while(1) {
        fread(&day, sizeof(day), 1, fp1);
        if(feof(fp1)) {
            break;
        }
        fwrite(&day, sizeof(day), 1, fp);
    }
    fclose(fp1);
    fclose(fp);

    remove(journal_path);
};

