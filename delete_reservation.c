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
#include "header_files/global/res_db_path.h"
#include "header_files/global/days_db_path.h"
#include "header_files/global/journal_sec_path.h"
/**********************************************
 * Color Initialisation and Terminal management 
 **********************************************/

/*******************************************************
 * My own libraries, collection of functions and structs
 ******************************************************/
#include "structures/room.h"
#include "structures/guest.h"
#include "structures/day.h"
#include "structures/reservations.h"

#include "header_files/delete_reservation.h"
#include "header_files/joins.h"
/* Collects the days which will be prosessed of Reservation res. */
void getResDatesToDelete(struct Reservation res) {

    struct Day day;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, daysdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate daysdb file getResDatesToDelete()");
        exit(127);
    }

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
    fclose(fp);

    deleteRoomFromDates(starting_point, finishing_point, res.room.id);
}
/* Removes res_room_id from days dy changing the days room_id[res_room_id] array to 1000000 + res_room_id.
    Remove from start day until finish. */
void deleteRoomFromDates(int start, int finish, int res_room_id) {

    struct Day day;
    FILE *fp, *fp1;
    char abs_path[PATH_LENGTH];
    char journal_path[PATH_LENGTH];
    joinHome(abs_path, daysdb);
    joinHome(journal_path, journal_sec);
    fp = fopen(abs_path, "rb");
    fp1 = fopen(journal_path, "wb");
    if (fp == NULL) {
        perror("Could not locate daysdb file deleteRoomFromDates()");
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
                day.room_id[res_room_id] = 100000 + res_room_id;
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
/* Deletes reservation from Database. */
void applyReservationDelete(int res_id) {

    struct Reservation res;
    FILE *fp, *fp1;
    char abs_path[PATH_LENGTH];
    char journal_path[PATH_LENGTH];
    joinHome(abs_path, reservationsdb);
    joinHome(journal_path, journal_sec);
    fp = fopen(abs_path, "rb");
    fp1 = fopen(journal_path, "wb");
    if (fp == NULL) {
        perror("Could not locate reservationsdb file applyReservationDelete()");
        fclose(fp1);
        remove(journal_path);
        exit(127);
    }

    while(1) {
        fread(&res, sizeof(res), 1, fp);
        if(feof(fp)) {
            break;
        } else if(res.id != res_id) {
            fwrite(&res, sizeof(res), 1, fp1);
        }
    }
    fclose(fp);
    fclose(fp1);

    fp = fopen(abs_path, "wb");
    fp1 = fopen(journal_path, "rb");
    while(1) {
         fread(&res, sizeof(res), 1, fp1);
        if(feof(fp1)) {
            break;
        } else {
            fwrite(&res, sizeof(res), 1, fp);
        }      
    }
    fclose(fp1);
    fclose(fp);

    remove(journal_path);
}

