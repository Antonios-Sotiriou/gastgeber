/********************
* Build in libraries
*********************/
#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
/*********************
 * Global constants 
 ********************/
#include "header_files/global/global_vars.h"
#include "header_files/global/res_db_path.h"
#include "header_files/global/days_db_path.h"
#include "header_files/global/rooms_db_path.h"
#include "header_files/global/guests_db_path.h"
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

#include "header_files/init_dbs.h"
#include "header_files/joins.h"
/* Creates the Days Database */
void createDaysDb() {

    struct Day day;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, daysdb);
    fp = fopen(abs_path, "rb");

    if(fp == NULL) {
        fp = fopen(abs_path, "wb");
        printf("Creating days Database.Please wait.\n");

        int year;
        char *months_nums[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
        char *months_names[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
        int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        char *week_days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

        int id_counter = 1;
        for(year = STARTING_YEAR; year <= FINISHING_YEAR; year++) {
            sprintf(day.year, "%d", year);

            if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
                month_days[1] = 29;
            } else {
                month_days[1] = 28;
            }

            int starting_day = get_first_day(year);

            for(int i = 0; i <= 11; i++) {

                int num_of_days = month_days[i];

                sprintf(day.month_num, "%s", months_nums[i]);
                sprintf(day.month_name, "%s", months_names[i]);

                for(int i = 1; i <= num_of_days; i++) {

                    day.id = id_counter;

                    sprintf(day.week_day, "%s", week_days[starting_day]);
                    starting_day++;

                    if(starting_day > 6) {
                        starting_day = 0;
                    }
                    if(i < 10) {
                        sprintf(day.day, "0%d", i);
                    } else {
                       sprintf(day.day, "%d", i);
                    }
                    sprintf(day.date, "%s/%s/%s", day.day, day.month_num, day.year);
                    id_counter++;
                    // Initializing day.room_id array with huge numbers which are gonna replaced by room IDs when a reservation takes place.
                    for(int x = 0; x <= TOTAL_ROOMS + 1; x++) {
                        day.room_id[x] = x + 1000000;
                    }
                    // Initializing day.res_ids array with huge numbers which are gonna replaced by reservation IDs when a reservation takes place.
                    for(int x = 0; x <= TOTAL_ROOMS + 1; x++) {
                        day.res_ids[x] = x + 1000000;
                    }
                    fwrite(&day, sizeof(day), 1, fp);
                }
            }
        }
        printf("Days database successfully created!\n");
    } else {
        printf("Days database already exists.\n");
    }
    fclose(fp);
}
/* Finds what day is the first day of a given year. */
int get_first_day(int year) {

    int day;
    day = (((year - 1) * 365) + ((year - 1) / 4) - ((year - 1) / 100) + ((year) / 400) + 1) % 7;
    return day;
}
/* Creates the Rooms Database */
void createRoomsDb() {

    struct Room room;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, roomsdb);
    fp = fopen(abs_path, "rb");

    if(fp == NULL) {
        fp = fopen(abs_path, "wb");
        printf("Creating rooms Database.Please wait.\n");
        for(int i = 1; i <= TOTAL_ROOMS; i++) {
            room.id = i;
            sprintf(room.name, "%s", "None");
            sprintf(room.type, "%s", "None");
            room.capacity = 0;
            room.price = 0.00;
            fwrite(&room, sizeof(room), 1, fp);
        }
        printf("Rooms database successfully created!\n");
    } else {
        printf("Rooms database already exists.\n");
    }
    fclose(fp);
}
/* Creates the Reservations Database */
void createReservationsDb() {

    struct Reservation reservation;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, reservationsdb);
    fp = fopen(abs_path, "rb");

    if(fp == NULL) {
        fp = fopen(abs_path, "wb");
        printf("Creating Reservations Database.Please wait.\n");
        
        reservation.id = 0;
        reservation.room.id = 0;
        reservation.guest.id = 0;
        sprintf(reservation.from_date, "%s", "None");
        sprintf(reservation.to_date, "%s", "None");
        fwrite(&reservation, sizeof(reservation), 1, fp);

        printf("Reservations database successfully created!\n");
    } else {
        printf("Reservations database already exists.\n");
    }
    fclose(fp);
}
/* Creates the Guests Database */
void createGuestsDb() {

    struct Guest guest;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, guestsdb);
    fp = fopen(abs_path, "rb");

    if(fp == NULL) {
        fp = fopen(abs_path, "wb");
        printf("Creating Guests Database.Please wait.\n");
        
        guest.id = 0;
        sprintf(guest.first_name, "%s", "None");
        sprintf(guest.last_name, "%s", "None");
        sprintf(guest.nationality, "%s", "None");
        guest.active = false;
        guest.repeated_guest = false;
        fwrite(&guest, sizeof(guest), 1, fp);

        printf("Guests database successfully created!\n");
    } else {
        printf("Guests database already exists.\n");
    }
    fclose(fp);
}
/* Returns a number which is greater by 1 than the last reservation ID.
    This number can be given as ID to the next applied reservation. */
int getNextReservationEntry() {

    struct Reservation reservation;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, reservationsdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate reservationsdb file getNextReservationEntry()");
        exit(127);
    } 
    // counter to find the last reservation record!
    int next_id = 0;
    while(1) {
        fread(&reservation, sizeof(reservation), 1, fp);
        if(feof(fp)) {
            break;            
        } else {
            next_id = reservation.id + 1;
        }
    }
    fclose(fp);
    return next_id;
}
/* Returns a number which is greater by 1 than the last guest ID.
    This number can be given as ID to the next applied guest. */
int getNextGuestEntry() {

    struct Guest guest;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, guestsdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate guestsdb file getNextGuestEntry()");
        exit(127);
    }
    // counter to find the last reservation record!
    int next_id = 0;
    while(1) {
        fread(&guest, sizeof(guest), 1, fp);
        if(feof(fp)) {
            break;            
        } else {
            next_id = guest.id + 1;
        }
    }
    fclose(fp);
    return next_id;
}

