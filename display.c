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

#include "header_files/tercon.h"
#include "header_files/display.h"

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

char *displayMainLogo() {

    clear_scr();
    Terminal term = tercon_init_rows_cols();
    char *positioning = malloc(sizeof(char));
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

    sprintf(positioning, "\x1b[%d;%dH", 22, (term.columns - 51) / 2);
    return positioning;
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
    printf("\x1b[%dG|     Guest ID       |     First Name     |      last Type     |    Nationality     |\n", (term.columns - 84) / 2);
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
    printf("\x1b[%dG|     Guest ID       |     First Name     |      last Type     |    Nationality     |\n", (term.columns - 84) / 2);
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
Terminal displayModifyLogo() {

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
    printf("\x1b[%d;%dH               >>> ", 17, (term.columns - 51) / 2);

    return term;
}
void displayModifyRoomLogo() {
    clear_scr();
    Terminal term = tercon_init_rows_cols();
    appLogo();
    printf("\x1b[%d;%dH\\             \x1b[32mRoom Modification Panel\x1b[0m             /\n", 10, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH \\        \x1b[32mChoose what you want to modify\x1b[0m         /\n", 11, (term.columns - 51) / 2);
    printf("\x1b[%d;%dH  '''''''''''''''''''''''''''''''''''''''''''''''\n\n", 12, (term.columns - 51) / 2);   
    displayErrorLog();
}
void displayModifyRoomChoices() {
    Terminal term = tercon_init_rows_cols();
    printf(ANSI_MOVE_CURSOR_COL "               1. Modify Room Name\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               2. Modify Room Type\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               3. Modify Room Capacity\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               4. Modify Room Price\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               5. Modify All\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               20. Go Back\n", (term.columns - 51) / 2);
    printf(ANSI_MOVE_CURSOR_COL "               >>> ", (term.columns - 51) / 2);
}
void displayModifyGuestLogo() {
    clear_scr();
    printf("*************************************\n");
    printf("*     Guest Modification Panel.     *\n");
    printf("*************************************\n\n");
}
void displayModifyGuestChoices() {
    printf("Choose what to modify...\n\n");
    printf("1. Modify Guest First Name\n");
    printf("2. Modify Guest Last Name\n");
    printf("3. Modify Guest Nationality\n");
    printf("4. Modify All\n");
    printf("0. Go Back\n\n");
}
void displayAllReservationsLogo() {
    clear_scr();
    printf("*************************************\n");
    printf("*      Display All Reservations.    *\n");
    printf("*************************************\n\n");
    printf(" --------------------------------------------------------------------------------------------------------\n");
    printf("|   Reservation ID   |      Room Id       |      Guest ID      |     From Date      |      To Date       |\n");
    printf(" --------------------------------------------------------------------------------------------------------\n");
}
void displayAllReservationsInfo(struct Reservation reservation) {
    printf("|");
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
    printf("---------------------------------------------------------------------------------------------------------\n");
}
void displayDeleteResLogo() {
    clear_scr();
    printf("*************************************\n");
    printf("*         Delete Reservation.       *\n");
    printf("*************************************\n\n");
}
void displayAnnuallyAvailabillityLogo() {
    clear_scr();
    printf("*************************************\n");
    printf("*       Annually Reservations.      *\n");
    printf("*************************************\n\n");   
}
void displayRoomsPerDay(struct Day st_arr[], int i) {
    
    for(int x = 0; x <= i - 1; x++) {
        char month[11];
        int month_days = 0;
        if(strcmp(st_arr[x].day, "01") == 0) {
            printf(ANSI_COLOR_GREEN "\n\n\t\t\t\t\t\t\t\t\t%s\n" ANSI_COLOR_RESET, st_arr[x].month_name);
            while (strcmp(st_arr[x].month_name, month) != 0) {
                sprintf(month, "%s", st_arr[x].month_name);
                for(int d = 0; d <= i - 1; d++) {
                    if(strcmp(st_arr[d].month_name, month) == 0) {
                        printf("| %s ", st_arr[d].day);
                        month_days++;
                    }
                }
            }
            printf("|\n");
            for(int m = 0; m <= month_days * 5; m++) {
                printf("-");
            }
            printf("\n|");
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
    printf("*************************************\n");
    printf("*    Annually Room Reservations.    *\n");
    printf("*************************************\n\n");   
}
void displayRoomAnnuallyReservationsInfo(int room_id, int input_year) {

    struct Day day;

    FILE *fp;
    fp = fopen(daysdb, "rb");
    
    int days_counter = 0;
    char str_year[5];
    sprintf(str_year, "%d", input_year);

    printf("Displaying Year %d\n", input_year);
    printf("Room Availabillity for Room ID: %d\n", room_id);

    while(1) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else if(strcmp(day.year, str_year) == 0) {
            if(strcmp(day.day, "01") == 0) {
                printf("\n|");
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

    printf("\n\nDays counter: %d\n\n", days_counter);
}
void displayAllRoomsAnnuallyReservationsLogo() {
    clear_scr();
    printf("*********************************************\n");
    printf("*    Annually Reservations for all Rooms    *\n");
    printf("*********************************************\n\n");   
}
void displayAllRoomsAnnuallyReservationsInfo(int input_year) {

    struct Day day;
    struct Room room;

    FILE *fp, *fp1, *fp2;
    fp = fopen(daysdb, "rb");
    fp1 = fopen(roomsdb, "rb");
    fp2 = fopen(output, "w");
    //fp2 = fopen("/dev/pts/1", "w");
    
    int days_counter = 0;
    char str_year[5];
    sprintf(str_year, "%d", input_year);

    printf("Displaying Year %d\n", input_year);
    fprintf(fp2, "Displaying Year %d\n", input_year);
    printf("|  Room IDs |");
    fprintf(fp2, "|  Room IDs |");

    while(1) {
        fread(&room, sizeof(room), 1, fp1);
        if(feof(fp1)) {
            break;
        } else {
            displayInt(room.id, 5);
            printf("|");
            fprintf(fp2, " %3d ", room.id);
            fprintf(fp2, "|");
        }
    }
    fclose(fp1);

    while(1) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else if(strcmp(day.year, str_year) == 0) {
            if(strcmp(day.day, "01") == 0) {
                printf("\n|");
                fprintf(fp2, "\n|");
                displayStr(day.month_name, 11);
                fprintf(fp2, " %9s ", day.month_name);
                printf("|");
                fprintf(fp2, "|");
            }
            printf("\n");
            fprintf(fp2, "\n");
            printf("|     %s    |", day.day);
            fprintf(fp2, "|     %2s    |", day.day);
            days_counter++;
            for(int i = 1; i <= TOTAL_ROOMS; i++) {
                if(day.room_id[i] > 0 && day.room_id[i] <= TOTAL_ROOMS) {
                    printf(" ");
                    fprintf(fp2, " ");
                    printf(ANSI_COLOR_GREEN_BG " X " ANSI_COLOR_RESET);
                    fprintf(fp2, " X ");
                    printf(" |");
                    fprintf(fp2, " |");
                } else {
                    printf("     |");
                    fprintf(fp2, "     |");
                }
            }
        }
    }
    fclose(fp);
    fclose(fp2);

    printf("\n\nDays counter: %d\n\n", days_counter);
}

void displayErrorLog() {

    Terminal term = tercon_init_rows_cols();

    printf("\x1b[%d;%dH ________________________________________________________________________________",term.rows - 3, (term.columns - 82) / 2);
    printf("\x1b[%d;%dH|                                                                                |",term.rows - 2, (term.columns - 82) / 2);
    printf("\x1b[%d;%dH|                                                                                |",term.rows - 1, (term.columns - 82) / 2);
    printf("\x1b[%d;%dH|                                                                                |",term.rows, (term.columns - 82) / 2);
}

