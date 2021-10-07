/********************
* Build in libraries
*********************/
#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
/*********************
 * Global constants 
 ********************/
#define STARTING_YEAR 2021
#define FINISHING_YEAR 2121
#define TOTAL_ROOMS 100
/*********************
 * Color Initialisation
 ********************/
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_GREEN_BG "\x1b[48;5;40m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
/*******************************************************
 * My own libraries, collection of functions and structs
 ******************************************************/
#include "input_functions/userInputFunctions.h"
#include "compare_functions/compareFunctions.h"
#include "structures/room.h"
#include "structures/guest.h"
#include "structures/day.h"
#include "structures/reservations.h"
#include "header_files/paths.h"
#include "init_dbs/init_databases.h"
#include "reserve_functions/reserveAvailabillity.h"
#include "delete_functions/deleteReservation.h"
#include "display_functions/displayFunctions.h"

/* functionality functions */
void reserve();
void displayRoom();
void displayAllRooms();
void modify();
void displayReservations();
void deleteReservation();
void displayAnnuallyAvailabillity();
void displayRoomAnnuallyReservations();
void displayAllRoomsAnnuallyReservations();

void main(int argc, char *argv[]) {

    if(argc > 1 && strcmp(argv[1], "init") == 0) {
        createDaysDb();
        createRoomsDb();
        createReservationsDb();
        printf("argv: %s", argv[1]);
        getc(stdin);
    }

    int choice;
    char c;

    while(1) {

        displayMainLogo();
        scanf("%d", &choice);

        switch(choice) {
            case 1 : reserve();
                break;
            case 2 : displayRoom();
                break;
            case 3 : displayAllRooms();
                break;
            case 5 : displayReservations();
                break;
            case 6 : deleteReservation();
                break;
            case 7 : displayAnnuallyAvailabillity();
                break;
            case 8 : displayRoomAnnuallyReservations();
                break;
            case 9 : displayAllRoomsAnnuallyReservations();
                break;
            case 0 : system("clear");
                exit(0);
                break;
            default :
                break;
        }
        while(c = getc(stdin) != '\n' && c != '\t');
    }
}

void reserve() {

    struct Reservation reservation;
    FILE *fp, *fp1;
    fp = fopen(reservationsdb, "rb");
    fp1 = fopen("/home/as/gastgeber/data/journal.dat", "ab");

    displayRoomReservationLogo();

    int room_id;
    int found = 0;
    // counter function which finds the last reservation record!
    int next_id = getNextEntry();

    char c;
    while(c = getc(stdin) != '\n' && c != '\t');

    while(found == 0) {

        printf("Enter Room ID(1-99):\n");  
        room_id = getnuminput(5);
        
        if (room_id < 1 || room_id > TOTAL_ROOMS) {
            system("clear");
            printf(ANSI_COLOR_RED "\nInvalid Room ID.\n" ANSI_COLOR_RESET);
            continue;
        } else {
            reservation.id = next_id;
            reservation.room.id = room_id;
            
            printf("Enter Guest ID: \n");
            scanf("%d", &reservation.guest.id);
            getc(stdin);
            int from_date = 0;
            while(from_date == 0) {
                printf("Room reserve from date: \n");
                if(getformatedDate(reservation.from_date) == 1 && checkFromDate(reservation) == 0) {
                    from_date = 1;
                }
            }
            int to_date = 0;
            while(to_date == 0) {
                printf("To date: \n");
                if(getformatedDate(reservation.to_date) == 1 && compareDates(reservation.from_date, reservation.to_date) == 1) {
                    if(checkAllDates(reservation) == 0) {
                        printf(ANSI_COLOR_GREEN "Room Succesfully reserved!\n\n" ANSI_COLOR_RESET);
                        to_date = 1;
                        fwrite(&reservation, sizeof(reservation), 1, fp1);
                    } else {
                        break;
                    }
                }
            }
            found = 1;
        }
    }
    fclose(fp);
    fclose(fp1);

    if(found == 1) {
        fp = fopen(reservationsdb, "ab");
        fp1 = fopen("/home/as/gastgeber/data/journal.dat", "rb");

        while(1) {
            fread(&reservation, sizeof(reservation), 1, fp1);
            if(feof(fp1)) {
                break;
            } 
            fwrite(&reservation, sizeof(reservation), 1, fp);
        }
    }
    fclose(fp1);
    fclose(fp);

    remove("/home/as/gastgeber/data/journal.dat");
    printf("Press Enter to continue...\n");
}

void displayRoom() {

    struct Room room;
    FILE *fp;
    fp = fopen(roomsdb, "rb");

    displayRoomInfoLogo();

    int room_id; 
    char c; 
    while(c = getc(stdin) != '\n' && c != '\t');
    
    printf("Enter Room ID(1-99):\n");  
    room_id = getnuminput(5);
        
    if (room_id < 1 || room_id > TOTAL_ROOMS) {
        printf(ANSI_COLOR_RED "\nInvalid Room ID.\n" ANSI_COLOR_RESET);
    } else {
        while(1) {
            fread(&room, sizeof(room), 1, fp);
            if(feof(fp)) {
                break;            
            } else if(room.id == room_id) {
                displayRoomInfo(room);
            }
        }
    }
    fclose(fp);

    printf("\nPress Enter to continue...\n");
}

void displayAllRooms() {

    struct Room room;
    FILE *fp;
    fp = fopen(roomsdb, "rb");

    char c; 
    while(c = getc(stdin) != '\n' && c != '\t');
    
    displayAllRoomsLogo();

    while(1) {
        fread(&room, sizeof(room), 1, fp);
        if(feof(fp)) {
            break;            
        } else {
            displayAllRoomsInfo(room);
        }
    }
    fclose(fp);

    printf("\nPress Enter to continue...\n");
}

void displayReservations() {

    struct Reservation res;
    FILE *fp;
    fp = fopen(reservationsdb, "rb");

    displayAllReservationsLogo();

    while(1) {
        fread(&res, sizeof(res), 1, fp);
        if(feof(fp)) {
            break;            
        } else {
            displayAllReservationsInfo(res);
        }
    }
    fclose(fp);
    printf("Press Enter to continue...\n");

    char c = getc(stdin);
}

void deleteReservation() {

    struct Reservation res;
    FILE *fp;
    fp = fopen(reservationsdb, "rb");

    char c;
    while(c = getc(stdin) != '\n' && c != '\t');

    displayDeleteResLogo();
    printf("Enter Reservation's id you want to delete: ");
    int res_id = getInteger(48, 10);
    int found = 0;

    while(found == 0) {
        fread(&res, sizeof(res), 1, fp);
        if(feof(fp)) {
            break;
        } else if(res.id == res_id && res_id != 0) {
            printf("Reservation's Id to be deleted: %d\n", res.id);
            printf("Reservation's Room ID: %d\n", res.room.id);
            printf("Reservation's Guests's ID: %d\n", res.guest.id);
            printf("Reservation's From Date: %s\n", res.from_date);
            printf("Reservation's To Date: %s\n", res.to_date);
            printf(ANSI_COLOR_RED "Are you sure you want to delete the Reservation: [Y/N]? " ANSI_COLOR_RESET);
            char c = getc(stdin);
            if(c == 'Y' || c == 'y') {
                getResDatesToDelete(res);
                printf("Deleting Reservation...\n\n");
                getc(stdin);
                found = 1;
            } else {
                printf(ANSI_COLOR_GREEN "Reservation deletion canceled." ANSI_COLOR_RESET);
                getc(stdin);
                found = -1;
            }
        }
    }
    fclose(fp);
    if(found == 0 && res_id != 0) {
        printf("No Reservation found with this ID: %d\n", found);
    } else if(found == 1) {
        applyReservationDelete(res_id);
        printf("Reservation has been deleted.\n");
    } else if(found == -1) {
        printf("\n");
    }

    printf("Press Enter to continue...\n");
}

void displayAnnuallyAvailabillity() {

    struct Day day;
    struct Day *st_arr;
    st_arr = malloc(sizeof(struct Day));
    
    FILE *fp;
    fp = fopen(daysdb, "rb");

    int input_year;
    char str_year[5];

    displayAnnuallyAvailabillityLogo();
    printf("Which year would you like to display? ");

    char c;
    while(c = getc(stdin) != '\n' && c != '\t');

    input_year = getInteger(48, 5);
    int i = 0;
    int dynamic_inc = 1;

    if((input_year < STARTING_YEAR && input_year != 0) || input_year > FINISHING_YEAR) {
        printf(ANSI_COLOR_RED "\nYear out of range: %d\n" ANSI_COLOR_RESET, input_year);
    } else if(input_year >= STARTING_YEAR && input_year <= FINISHING_YEAR) {
        sprintf(str_year, "%d", input_year);
        while(1) {
            fread(&day, sizeof(day), 1, fp);
            if(feof(fp)) {
                break;
            } else if(strcmp(day.year, str_year) == 0) {
                // Dynamically allocate memory for the array and create the struct array.
                st_arr = realloc(st_arr, sizeof(struct Day) * dynamic_inc);
                st_arr[i] = day;
                i++;
                // create a multiplier for each item to increase array size accordingly.
                dynamic_inc++;
            }
        }
    } else if(input_year == 0) {
        printf(ANSI_COLOR_RED "\nUndefined year parsing...!\n" ANSI_COLOR_RESET);
    }
    fclose(fp);
    // Process and display.
    displayRoomsPerDay(st_arr, i);
    printf("\nPress Enter to continue...");
}

void displayRoomAnnuallyReservations() {

    displayRoomAnnuallyReservationsLogo();

    char c;
    while(c = getc(stdin) != '\n' && c != '\t');

    printf("Enter room ID you want to display: ");
    int room_id = getInteger(48, 5);
    if(room_id <= 0 || room_id > TOTAL_ROOMS) {
        printf(ANSI_COLOR_RED "\nProvide a correct room ID.\nYou can check the room IDs from option 3 in main menu.\n\n" ANSI_COLOR_RESET);
    } else {
        printf("Room ID: %d\n", room_id);
        printf("Enter which year you want to display: ");
        int input_year = getInteger(48, 5);
        if ((input_year < STARTING_YEAR && input_year != 0) || input_year > FINISHING_YEAR) {
            printf(ANSI_COLOR_RED "\nProvide a correct year.\nAre you sure year is in range of Software STARTING_YEAR - FINISHING_YEAR?\n\n" ANSI_COLOR_RESET);
        } else if(input_year >= STARTING_YEAR && input_year <= FINISHING_YEAR) {
            printf("Year: %d\nDisplaying Room Annually Reservations...\n\n", input_year);
            displayRoomAnnuallyReservationsInfo(room_id, input_year);
        }
    }

    printf("Press Enter to continue...");
}

void displayAllRoomsAnnuallyReservations() {

    displayAllRoomsAnnuallyReservationsLogo();

    char c; 
    while(c = getc(stdin) != '\n' && c != '\t');
}