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

/*******************************************************
 * My own libraries, collection of functions and structs
 ******************************************************/
#include "header_files/paths.h"

#include "structures/room.h"
#include "structures/guest.h"
#include "structures/day.h"
#include "structures/reservations.h"

#include "header_files/delete_reservation.h"

void getResDatesToDelete(struct Reservation res) {

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
    fclose(fp);

    deleteRoomFromDates(starting_point, finishing_point, res.room.id);
}

void deleteRoomFromDates(int start, int finish, int res_room_id) {

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
                day.room_id[res_room_id] = 100000 + res_room_id;
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

void applyReservationDelete(int res_id) {

    struct Reservation res;
    FILE *fp, *fp1;

    fp = fopen(reservationsdb, "rb");
    fp1 = fopen(journal_sec, "wb");
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

    fp1 = fopen(journal_sec, "rb");
    fp = fopen(reservationsdb, "wb");
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

    remove(journal_sec);
}

