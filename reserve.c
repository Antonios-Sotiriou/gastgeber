/********************
* Build in libraries
*********************/
#include <stdio.h>
#include <string.h>
/*********************
 * Global constants 
 ********************/
#include "header_files/global_vars.h"
/**********************************************
 * Color Initialisation and Terminal management 
 **********************************************/
#include "header_files/tercon.h"
/*******************************************************
 * My own libraries, collection of functions and structs
 ******************************************************/
#include "header_files/paths.h"

#include "structures/room.h"
#include "structures/guest.h"
#include "structures/day.h"
#include "structures/reservations.h"

#include "header_files/reserve.h"

int checkFromDate(struct Reservation res) {

    struct Day day;
    FILE *fp;
    fp = fopen(daysdb, "rb");

    int starting_point;
    // While loop to find the startind day;
    while(1) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else if(strcmp(day.date, res.from_date) == 0) {
            starting_point = day.id;
            for(int i = 1; i <= sizeof(day.room_id) / sizeof(int); i++) {
                // Check if the room id is already in the days rooms ids.
                if(day.room_id[i] == res.room.id) {
                    printf(ANSI_COLOR_RED "Room is already reserved for this date: %s\n" ANSI_COLOR_RESET, day.date);
                    return 1;
                }
            }
        }
    }
    fclose(fp);
    
    if(starting_point <= 0 || starting_point > 36500) {
        printf("Check the dates please.\nMaybe you Provided a day that is not exists in this year.\nFor example 29 of February or 31 of November.\n");
        return 1;
    }
    return 0;
}

int checkAllDates(struct Reservation res) {

    struct Day day;
    FILE *fp;
    fp = fopen(daysdb, "rb");

    int starting_point;
    int finishing_point;
    // While loop to find the startind and ending days ids;
    while(1) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else if(strcmp(day.date, res.from_date) == 0) {
            starting_point = day.id;
        } else if(strcmp(day.date, res.to_date) == 0) {
            finishing_point = day.id;
        }
    }

    if(finishing_point <= 0 || finishing_point > 36500) {
        printf("Check the dates please.\nMaybe you Provided a day that is not exists in this year.\nFor example 29 of February or 31 of November.\n");
        return 1;
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
                       printf(ANSI_COLOR_RED "Room is already reserved for those dates: %s\n" ANSI_COLOR_RESET, day.date);
                       num_of_days++;
                    }
                }
            }
        }
    }
    fclose(fp);

    if(num_of_days != 0) {
        return 1;
    } else {
        printf("Shall the reservation be applied: [Y/N]? ");
        char c = getc(stdin);
        if(c == 'Y' || c == 'y') {
            applyReservation(starting_point, finishing_point, res.room.id);
            printf(ANSI_COLOR_GREEN "\nReservation applied success.\n" ANSI_COLOR_RESET);
            getc(stdin);
            return 0;
        } else {
            printf(ANSI_COLOR_RED "\nReservation Canceled!\n" ANSI_COLOR_RESET);
            getc(stdin);
            return 1;
        }
    }
}

void applyReservation(int start, int finish, int res_room_id) {

    struct Day day;
    FILE *fp, *fp1;
    fp = fopen(daysdb, "rb");
    fp1 = fopen(journal_sec, "wb");

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

    fp = fopen(daysdb, "wb");
    fp1 = fopen(journal_sec, "rb");

    while(1) {
        fread(&day, sizeof(day), 1, fp1);
        if(feof(fp1)) {
            break;
        }
        fwrite(&day, sizeof(day), 1, fp);
    }
    fclose(fp1);
    fclose(fp);

    remove(journal_sec);
}

