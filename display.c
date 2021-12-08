/********************
* Build in libraries
*********************/
#ifdef _WIN32
    #define clear_scr() system("cls")
#else
    #define clear_scr() system("clear")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/*********************
 * Global constants 
 ********************/
#include "header_files/global/global_vars.h"
#include "header_files/global/days_db_path.h"
#include "header_files/global/rooms_db_path.h"
#include "header_files/global/output_path.h"
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

#include "header_files/tercon.h"
#include "header_files/display.h"
#include "header_files/joins.h"

void appLogo() {
    Terminal term = tercon_init_rows_cols();
    printf("\x1b[%d;%dH***************************************************\n", 1, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH_________   /  \\   |        |____________         \n", 2, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH  _____  | /    \\  |    ____|            |        \n", 3, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH |    |__|/  /\\  \\ |  |___|_ ___    _____|       \n", 4, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH |  _____/  /__\\  \\ ______  |  |   |             \n", 5, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH | |___  | _____   \\ _____| |  |   |              \n", 6, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH |____|  |/     \\   \\       |  |   | Reservations\n", 7, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH_________|       \\___\\______|  |___|     Software\n", 8, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH***************************************************\n", 9, (term.columns - 51) / 2);
}

void displayMainLogo() {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH          1. Reserve a room                        \n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH          2. Room info                             \n", 11, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH          3. All rooms                             \n", 12, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH          4. Guest info                            \n", 13, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH          5. All Guests                            \n", 14, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH          6. Modify Room or Guest                  \n", 15, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH          7. All Reservations                      \n", 16, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH          8. Delete room Reservation               \n", 17, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH          9. Annually Availabillity                \n", 18, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH          10. Room Annually Reservations           \n", 19, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH          11. all Rooms Annually Reservations      \n", 20, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH          20. Exit                                 \n", 21, (term.columns -51) / 2);
    displayErrorLog();
    printf("\x1b[%d;%dH          >>> ", 22, (term.columns - 51) / 2);
}
Terminal displayRoomReservationLogo() { 
    clear_scr();
    Terminal term;
    term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\                \x1b[32mReservations Panel\x1b[0m               /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH '''''''''''''''''''''''''''''''''''''''''''''''''\n", 11, (term.columns - 51) / 2);
    displayErrorLog();

    return term;
}
void displayPreviousStep(struct Reservation res) {
    // Importand function to be used in reserve() primary function.Redisplays previous page 
    // if the user received a < room is already received for NUM of days > error.Error days must be
    // more than 2 to use this function.For less days is not needed because terminal isn't getting messy. 
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    term.cursor_y = 16;
    appLogo();
    printf("\x1b[%d;%dH\\                \x1b[32mReservations Panel\x1b[0m               /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH '''''''''''''''''''''''''''''''''''''''''''''''''\n", 11, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH  Enter Room ID(%d-%d): %d", 13, (term.columns - 51) / 2, 1, TOTAL_ROOMS, res.room.id);
    printf("\x1b[%d;%dH  Guest First Name: %s",14, (term.columns - 51) / 2, res.guest.first_name);
    printf("\x1b[%d;%dH  Guest Last Name: %s",15, (term.columns - 51) / 2, res.guest.last_name);
    if (!res.guest.repeated_guest) {
        printf("\x1b[%d;%dH  Guest Nationality: %s",term.cursor_y, (term.columns - 51) / 2, res.guest.nationality);
        term.cursor_y++;
    }
    printf("\x1b[%d;%dH  Room reserve from date: %s",term.cursor_y, (term.columns - 51) / 2, res.from_date);
    term.cursor_y++;
    displayErrorLog();
    printf("\x1b[%d;%dH  To date: ", term.cursor_y, (term.columns - 51) / 2);
}
void displayRoomInfoLogo() {
    clear_scr();
    Terminal term;
    term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\          \x1b[32mDisplaying Room Informations\x1b[0m           /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH '''''''''''''''''''''''''''''''''''''''''''''''''\n", 11, (term.columns - 51) / 2);   
    displayErrorLog();
}
void displayRoomInfo(struct Room room) {
    Terminal term = tercon_init_rows_cols();
    printf("\x1b[%dG --------------------------------------------------------------------------------------------------------\n", (term.columns - 105) / 2);
    printf("\x1b[%dG|      Room ID       |     Room Name      |      Room Type     |      Capacity      |       Price        |\n", (term.columns - 105) / 2);
    printf("\x1b[%dG --------------------------------------------------------------------------------------------------------\n", (term.columns - 105) / 2);
    printf("\x1b[%dG|", (term.columns - 105) / 2);
    displayInt(room.id, 20);
    printf("|");
    displayStr(room.name, 20);
    printf("|");
    displayStr(room.type, 20);
    printf("|");
    displayInt(room.capacity, 20);
    printf("|");
    displayInt(room.price, 20);
    printf("|\n");
    printf("\x1b[%dG --------------------------------------------------------------------------------------------------------\n", (term.columns - 105) / 2);
}
void displayAllRoomsLogo() { 
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\       \x1b[32mDisplaying All Room Informations\x1b[0m          /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH '''''''''''''''''''''''''''''''''''''''''''''''''\n", 11, (term.columns - 51) / 2);  
    printf("\x1b[%dG --------------------------------------------------------------------------------------------------------\n", (term.columns - 105) / 2);
    printf("\x1b[%dG|      Room ID       |     Room Name      |      Room Type     |      Capacity      |       Price        |\n", (term.columns - 105) / 2);
    printf("\x1b[%dG --------------------------------------------------------------------------------------------------------\n", (term.columns - 105) / 2);
}
void displayAllRoomsInfo(struct Room room) {
    Terminal term = tercon_init_rows_cols();
    printf("\x1b[%dG|", (term.columns - 105) / 2);
    displayInt(room.id, 20);
    printf("|");
    displayStr(room.name, 20);
    printf("|");
    displayStr(room.type, 20);
    printf("|");
    displayInt(room.capacity, 20);
    printf("|");
    displayInt(room.price, 20);
    printf("|\n");
    printf("\x1b[%dG --------------------------------------------------------------------------------------------------------\n", (term.columns - 105) / 2);
}
void displayGuestInfoLogo() {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\          \x1b[32mDisplaying Guest Informations\x1b[0m          /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH '''''''''''''''''''''''''''''''''''''''''''''''''\n", 11, (term.columns - 51) / 2);  
    displayErrorLog(); 
}
void displayGuestInfo(struct Guest guest) {
    Terminal term = tercon_init_rows_cols();
    printf("\x1b[%dG -----------------------------------------------------------------------------------\n", (term.columns - 84) / 2);
    printf("\x1b[%dG|     Guest ID       |     First Name     |      last Name     |    Nationality     |\n", (term.columns - 84) / 2);
    printf("\x1b[%dG -----------------------------------------------------------------------------------\n", (term.columns - 84) / 2);
    printf("\x1b[%dG|", (term.columns - 84) / 2);
    displayInt(guest.id, 20);
    printf("|");
    displayStr(guest.first_name, 20);
    printf("|");
    displayStr(guest.last_name, 20);
    printf("|");
    displayStr(guest.nationality, 20);
    printf("|\n");
    printf("\x1b[%dG -----------------------------------------------------------------------------------\n", (term.columns - 84) / 2);
}
void displayAllGuestsLogo() {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\       \x1b[32mDisplaying All Guests Informations\x1b[0m        /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH '''''''''''''''''''''''''''''''''''''''''''''''''\n\n", 11, (term.columns - 51) / 2);  
    printf("\x1b[%dG -----------------------------------------------------------------------------------\n", (term.columns - 84) / 2);
    printf("\x1b[%dG|     Guest ID       |     First Name     |      last Name     |    Nationality     |\n", (term.columns - 84) / 2);
    printf("\x1b[%dG -----------------------------------------------------------------------------------\n", (term.columns - 84) / 2);
}
void displayAllGuestsInfo(struct Guest guest) {
    Terminal term = tercon_init_rows_cols();
    printf("\x1b[%dG|", (term.columns - 84) / 2);
    displayInt(guest.id, 20);
    printf("|");
    displayStr(guest.first_name, 20);
    printf("|");
    displayStr(guest.last_name, 20);
    printf("|");
    displayStr(guest.nationality, 20);
    printf("|\n");
    printf("\x1b[%dG -----------------------------------------------------------------------------------\n", (term.columns - 84) / 2); 
}
void displayModifyLogo() {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\               \x1b[32mModification Panel\x1b[0m                /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH \\        \x1b[32mChoose what you want to modify\x1b[0m         /\n", 11, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH  '''''''''''''''''''''''''''''''''''''''''''''''\n\n", 12, (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               1. Modify a room\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               2. Modify Guest Info\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               20. Go Back\n", (term.columns - 51) / 2);
    displayErrorLog();
    printf("\x1b[%d;%dH\x1b[2K               >>> ", 17, (term.columns - 51) / 2);
}
void displayModifyRoomLogo() {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\             \x1b[32mRoom Modification Panel\x1b[0m             /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH '''''''''''''''''''''''''''''''''''''''''''''''''\n\n", 11, (term.columns - 51) / 2);
    displayErrorLog();
}
void displayModifyRoomChoices(struct Room room) {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    displayModifyRoomLogo();
    printf(ANSI_MOVE_CURSOR_TO "Modifying Room: %d\n", 13, (term.columns - 16) / 2, room.id);
    displayRoomInfo(room);
    printf(ANSI_MOVE_CURSOR_COL "               1. Modify Room Name\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               2. Modify Room Type\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               3. Modify Room Capacity\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               4. Modify Room Price\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               5. Modify All\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               20. Go Back\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL ANSI_ERASE_LINE "               >>> ", (term.columns - 51) / 2);
}
void displayModifyGuestLogo() {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\             \x1b[32mGuest Modification Panel\x1b[0m            /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH '''''''''''''''''''''''''''''''''''''''''''''''''\n\n", 11, (term.columns - 51) / 2);
    displayErrorLog();
}
void displayModifyGuestChoices(struct Guest guest) {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    displayModifyGuestLogo();
    printf(ANSI_MOVE_CURSOR_TO "Modifying Guest: %d\n", 13, (term.columns - 17) / 2, guest.id);
    displayGuestInfo(guest);
    printf(ANSI_MOVE_CURSOR_COL "               1. Modify First Name\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               2. Modify Last Name\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               3. Modify Nationality\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               4. Modify All\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               20. Go Back\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL ANSI_ERASE_LINE "               >>> ", (term.columns - 51) / 2);
}
void displayAllReservationsLogo() {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\           \x1b[32mDisplaying All Reservations\x1b[0m           /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH '''''''''''''''''''''''''''''''''''''''''''''''''\n\n", 11, (term.columns - 51) / 2); 
    printf("\x1b[%dG --------------------------------------------------------------------------------------------------------\n", (term.columns - 105) / 2);
    printf("\x1b[%dG|   Reservation ID   |      Room Id       |      Guest ID      |     From Date      |      To Date       |\n", (term.columns - 105) / 2);
    printf("\x1b[%dG --------------------------------------------------------------------------------------------------------\n", (term.columns - 105) / 2);
}
void displayAllReservationsInfo(struct Reservation reservation) {
    Terminal term = tercon_init_rows_cols();
    printf("\x1b[%dG|", (term.columns - 105) / 2);
    displayInt(reservation.id, 20);
    printf("|");
    displayInt(reservation.room.id, 20);
    printf("|");
    displayInt(reservation.guest.id, 20);
    printf("|");
    displayStr(reservation.from_date, 20);
    printf("|");
    displayStr(reservation.to_date, 20);
    printf("|\n");
    printf("\x1b[%dG---------------------------------------------------------------------------------------------------------\n", (term.columns - 105) / 2);
}
void displayDeleteResLogo() {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\               \x1b[32mDelete Reservation\x1b[0m                /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH '''''''''''''''''''''''''''''''''''''''''''''''''\n\n", 11, (term.columns - 51) / 2);
    displayErrorLog();
    tercon_move_y_x(13, 0);
}
void displayReservationInfo(struct Reservation reservation) {
    Terminal term = tercon_init_rows_cols();
    printf("\x1b[%dG --------------------------------------------------------------------------------------------------------\n", (term.columns - 105) / 2);
    printf("\x1b[%dG|   Reservation ID   |      Room Id       |      Guest ID      |     From Date      |      To Date       |\n", (term.columns - 105) / 2);
    printf("\x1b[%dG --------------------------------------------------------------------------------------------------------\n", (term.columns - 105) / 2);
    printf("\x1b[%dG|", (term.columns - 105) / 2);
    displayInt(reservation.id, 20);
    printf("|");
    displayInt(reservation.room.id, 20);
    printf("|");
    displayInt(reservation.guest.id, 20);
    printf("|");
    displayStr(reservation.from_date, 20);
    printf("|");
    displayStr(reservation.to_date, 20);
    printf("|\n");
    printf("\x1b[%dG---------------------------------------------------------------------------------------------------------\n", (term.columns - 105) / 2);
}
void displayAnnuallyAvailabillityLogo() {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\             \x1b[32mAnnually Availabillity\x1b[0m              /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH '''''''''''''''''''''''''''''''''''''''''''''''''\n\n", 11, (term.columns - 51) / 2);
    displayErrorLog();
}
void displayRoomsPerDay(struct Day st_arr[], int i) {
    
    Terminal term = tercon_init_rows_cols();
    for(int x = 0; x <= i - 1; x++) {
        char month[11];
        int month_days = 0;
        if(strcmp(st_arr[x].day, "01") == 0) {
            printf("\n");
            printf(ANSI_COLOR_GREEN "\x1b[%dG%s\n" ANSI_COLOR_RESET, abs((term.columns - strlen(st_arr[x].month_name)) / 2), st_arr[x].month_name);
            printf(ANSI_MOVE_CURSOR_COL, (term.columns - 156) / 2);
            while (strcmp(st_arr[x].month_name, month) != 0) {
                sprintf(month, "%s", st_arr[x].month_name);
                for(int d = 0; d <= i - 1; d++) {
                    if(strcmp(st_arr[d].month_name, month) == 0) {
                        printf("| %s ",  st_arr[d].day);
                        month_days++;
                    }
                }
            }
            printf("|\n");
            printf(ANSI_MOVE_CURSOR_COL, (term.columns - 156) / 2);
            for(int m = 0; m <= month_days * 5; m++) {
                printf("-");
            }
            printf("\n\x1b[%dG|", (term.columns - 156) / 2);
        }
        int num_of_rooms = 0;
        for(int z = 0; z <= sizeof(st_arr[x].room_id) / sizeof(int) - 1; z++) {
            if(st_arr[x].room_id[z] >= 1 && st_arr[x].room_id[z] <= TOTAL_ROOMS) {
                num_of_rooms++;
            }
        }
        if(num_of_rooms == 0) {
            printf(ANSI_COLOR_RED " %d  " ANSI_COLOR_RESET, num_of_rooms);
            printf("|");
        } else {
            displayInt(num_of_rooms, 4);
            printf("|");
        }
    }
    printf("\n");
    free(st_arr);  
}
void displayInt(int id, int dis_len) {

    char converted[30];

    sprintf(converted, "%d", id);
    int d = (dis_len - strlen(converted)) / 2;

    for(int i = 0; i <= dis_len - strlen(converted); i++) {
        if(i < d) {
            printf(" ");
        } else if(i > d) {
            printf(" ");
        } else {
            printf(ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET, converted);
        }
    }
}
void displayStr(char *str, int dis_len) {

    int d = (dis_len - strlen(str)) / 2;

    for(int i = 0; i <= dis_len - strlen(str); i++) {
        if(i < d) {
            printf(" ");
        } else if(i > d) {
            printf(" ");
        } else {
            printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, str);
        }
    }
}
void displayRoomAnnuallyReservationsLogo() {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\           \x1b[32mRoom Annually Reservations\x1b[0m            /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH '''''''''''''''''''''''''''''''''''''''''''''''''\n\n", 11, (term.columns - 51) / 2);
    displayErrorLog();
}
void displayRoomAnnuallyReservationsInfo(int room_id, int input_year) {

    Terminal term = tercon_init_rows_cols();
    struct Day day;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, daysdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate daysdb file displayRoomAnnuallyReservationInfo()");
        exit(127);
    }
    
    int days_counter = 0;
    char str_year[5];
    sprintf(str_year, "%d", input_year);

    printf(ANSI_MOVE_CURSOR_TO "Displaying Year %d\n", 13, (term.columns - 20) / 2, input_year);
    printf(ANSI_MOVE_CURSOR_TO "Room Availabillity for Room ID: %d\n", 14, (term.columns - 36) / 2, room_id);
    while(1) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else if(strcmp(day.year, str_year) == 0) {
            if(strcmp(day.day, "01") == 0) {
                printf("\n");
                printf(ANSI_MOVE_CURSOR_COL "|", (term.columns - 106) / 2);
                displayStr(day.month_name, 11);
                printf("|");
            }
            days_counter++;
            int room_booked = 0;
            for(int i = 0; i <= sizeof(day.room_id) / sizeof(int); i++) {
                if(day.room_id[i] == room_id) {
                    room_booked = 1;
                    break;
                }
            }
            if(room_booked == 1) {
                printf(ANSI_COLOR_GREEN_BG "%s" ANSI_COLOR_RESET, day.day);
                printf("|");
            } else {
                printf("%s", day.day);
                printf("|");
            }
        }
    }
    fclose(fp);

    printf("\n\n");
    printf(ANSI_MOVE_CURSOR_COL "Days counter: %d\n", (term.columns - 18) / 2, days_counter);
}
void displayAllRoomsAnnuallyReservationsLogo() {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\         \x1b[32mAll Rooms Annually Reservations\x1b[0m         /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH '''''''''''''''''''''''''''''''''''''''''''''''''\n\n", 11, (term.columns - 51) / 2);
    displayErrorLog();
}
void displayAllRoomsAnnuallyReservationsInfo(struct Room rooms_arr[], int input_year) {

    struct Day day;
    FILE *fp, *fp1;
    char abs_path_a[PATH_LENGTH];
    char abs_path_b[PATH_LENGTH];
    joinHome(abs_path_a, daysdb);
    joinHome(abs_path_b, output);
    fp = fopen(abs_path_a, "rb");
    fp1 = fopen(abs_path_b, "w");
    if (fp == NULL) {
        perror("Could not locate file displayAllRoomsAnnuallyReservationsInfo()");
        fclose(fp1);
        exit(127);
    }

    int days_counter = 0;
    char str_year[5];
    sprintf(str_year, "%d", input_year);

    Terminal term = tercon_init_rows_cols();
    printf("Displaying Year %d\n", input_year);
    fprintf(fp1, "Displaying Year %d\n", input_year);
    printf("|  Room IDs |");
    fprintf(fp1, "|  Room IDs |");

    for (int i = 0; i <= TOTAL_ROOMS - 1; i++) {
        displayInt(rooms_arr[i].id, 5);
        printf("|");
        fprintf(fp1, " %3d ", rooms_arr[i].id);
        fprintf(fp1, "|");
    }

    while(1) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else if(strcmp(day.year, str_year) == 0) {
            if(strcmp(day.day, "01") == 0) {
                printf("\n|");
                fprintf(fp1, "\n|");
                displayStr(day.month_name, 11);
                fprintf(fp1, " %9s ", day.month_name);
                printf("|");
                fprintf(fp1, "|");
            }
            printf("\n");
            fprintf(fp1, "\n");
            printf("|     %s    |", day.day);
            fprintf(fp1, "|     %2s    |", day.day);
            days_counter++;
            for(int i = 1; i <= TOTAL_ROOMS; i++) {
                if(day.room_id[i] > 0 && day.room_id[i] <= TOTAL_ROOMS) {
                    printf(" ");
                    fprintf(fp1, " ");
                    printf(ANSI_COLOR_GREEN_BG " X " ANSI_COLOR_RESET);
                    fprintf(fp1, " X ");
                    printf(" |");
                    fprintf(fp1, " |");
                } else {
                    printf("     |");
                    fprintf(fp1, "     |");
                }
            }
        }
    }
    fclose(fp);
    fclose(fp1);

    printf(ANSI_MOVE_CURSOR_COL "Days counter: %d\n", (term.columns - 18) / 2, days_counter);
}

void displayErrorLog() {
    Terminal term = tercon_init_rows_cols();
    printf("\x1b[%d;%dH ________________________________________________________________________________",term.rows - 3, (term.columns - 82) / 2);
    printf("\x1b[%d;%dH|                                                                                |",term.rows - 2, (term.columns - 82) / 2);
    printf("\x1b[%d;%dH|                                                                                |",term.rows - 1, (term.columns - 82) / 2);
    printf("\x1b[%d;%dH|                                                                                |",term.rows, (term.columns - 82) / 2);
}

