/********************
* Build in libraries
*********************/
#ifdef _WIN32
    #include <Windows.h>
    #define clear_scr() system("cls")
#else
    #include <unistd.h>
    #define clear_scr() system("clear")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
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

#include "header_files/userinput.h"
#include "header_files/display.h"
#include "header_files/comparedates.h"
#include "header_files/init_dbs.h"
#include "header_files/reserve.h"
#include "header_files/delete_reservation.h"
#include "header_files/modify.h"
#include "header_files/handle_guest.h"

/* functionality functions */
void reserve();
void displayRoom();
void displayAllRooms();
void displayGuest();
void displayAllGuests();
void modify();
void displayReservations();
void deleteReservation();
void displayAnnuallyAvailabillity();
void displayRoomAnnuallyReservations();
void displayAllRoomsAnnuallyReservations();
void main_error();

int main(int argc, char *argv[]) {

    if(argc > 1 && strcmp(argv[1], "init") == 0) {
        // Database initialization functions.Can be found in init_dbs.c file.
        createDaysDb();
        createRoomsDb();
        createReservationsDb();
        createGuestsDb();
    }

    int choice;

    while(1) {
        // All the functions that start with the word [ display ] and are not part of the following
        // [ switch ],can be found in display.c file.
        char *positioning = displayMainLogo();
        printf("%s          >>> ", positioning);
        // getInteger(int b_l, int i_l) function to get an integer from a buffer that is 48 bytes longand integer has length 4 digits.
        // Can be found in userinput.c
        choice = getInteger(48, 4);

        // All the [ switch ] functions can be found in this file with this order.
        switch(choice) {
            case 1 : reserve();
                break;
            case 2 : displayRoom();
                break;
            case 3 : displayAllRooms();
                break;
            case 4 : displayGuest();
                break;
            case 5 : displayAllGuests();
                break;
            case 6 : modify();
                break;
            case 7 : displayReservations();
                break;
            case 8 : deleteReservation();
                break;
            case 9 : displayAnnuallyAvailabillity();
                break;
            case 10 : displayRoomAnnuallyReservations();
                break;
            case 11 : displayAllRoomsAnnuallyReservations();
                break;
            case 20 : clear_scr();
                exit(0);
                break;
            case -1 :
                // Error handling and error message printing for invalid integer length.See bottom of the file.
                main_error();
                break;
            case -2 :
                // Error handling and error message printing for no integer input.See bottom of the file.
                main_error();
                break;
            default : 
                break;
        }
        free(positioning);
    }
    return 1;
}

void reserve() {

    struct Reservation reservation;
    FILE *fp;
    fp = fopen(reservationsdb, "ab");

    Terminal term = displayRoomReservationLogo();
    term.cursor_x = 0;
    term.cursor_y = 13;
    int num_of_days = 0;

    int room_id;
    int found = 0;
    // counter function which finds the last reservation record! Can be found in init_dbs.c file
    int next_id = getNextReservationEntry();

    while(found == 0) {

        printf("\x1b[%d;%dH  Enter Room ID(%d-%d): ", term.cursor_y, (term.columns - 51) / 2, 1, TOTAL_ROOMS); 
        term.cursor_y += 1;
        room_id = getnuminput(5);
        
        if (room_id < 1 || room_id > TOTAL_ROOMS) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid Room ID.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 16) / 2);
            break;
        } else {
            reservation.id = next_id;
            reservation.room.id = room_id;
            // Check if guest already exists,if exists he must be marked as Repeated Guest else if not create after success reservation.
            reservation.guest = handleGuest();
            if (!reservation.guest.active) {
                break;
            }

            int from_date = 0;
            while(from_date == 0) {
                if (reservation.guest.repeated_guest) {
                    tercon_move_y_x(16, (term.columns - 51 ) /2);
                    term.cursor_y = 16;
                } else {
                    tercon_move_y_x(17, (term.columns - 51 ) /2);
                    term.cursor_y = 17;
                }
                printf(ANSI_ERASE_LINE "  Room reserve from date: ");
                if(getformatedDate(reservation.from_date) == 1 && checkFromDate(reservation) == 0) {
                    from_date = 1;
                } else {
                    tercon_move_y_x(term.rows - 4, (term.columns - 26 ) / 2);
                    printf(ANSI_ERASE_LINE "Do you want to retry? [Y/N]: ");
                    char c = getc(stdin);
                    if (c == 'y' || c == 'Y') {
                        tercon_clear_error_log();
                        tercon_move_y_x(term.rows - 4, (term.columns - 26 ) / 2);
                        printf("\x1b[2K");
                        getc(stdin);
                        continue;
                    } else {
                        return;
                    }
                }
            }
            int to_date = 0;
            while(to_date == 0) {
                tercon_move_y_x(term.cursor_y + 1, (term.columns - 51 ) /2);
                printf(ANSI_ERASE_LINE "  To date: ");
                int step_count = 0;
                if(getformatedDate(reservation.to_date) == 1 && compareDates(reservation.from_date, reservation.to_date) == 1) {
                    // we need the num_of_days here to position the cursor if the days are overflow the error printing area.
                    num_of_days = checkAllDates(reservation);
                    if(num_of_days == 0) {
                        printf(ANSI_COLOR_GREEN "\x1b[%d;%dHRoom Succesfully reserved!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 26) / 2);
                        // Write also the Guest to guests database.
                        createGuestEntry(reservation);
                        to_date = 1;
                        fwrite(&reservation, sizeof(reservation), 1, fp);
                    } else {
                        step_count = 1;
                    }
                } else {
                    step_count = 1;
                }
                if (step_count != 0) {
                    tercon_move_y_x(term.rows - 4, (term.columns - 26 ) / 2);
                    printf(ANSI_ERASE_LINE "Do you want to retry? [Y/N]: ");
                    char c = getc(stdin);
                    if (c == 'y' || c == 'Y') {
                        tercon_clear_error_log();
                        tercon_move_y_x(term.rows - 4, (term.columns - 26 ) / 2);
                        printf("\x1b[2K");
                        getc(stdin);
                        continue;
                    } else {
                        return;
                    }
                }
            }
            found = 1;
        }
    }
    fclose(fp);

    // num_of_days is usefull here to position the cursor dynamically.
    if (num_of_days > 1) {
        tercon_move_y_x((term.rows - 4) - num_of_days, (term.columns - 26 ) / 2);
    } else {
        tercon_move_y_x(term.rows - 4, (term.columns - 26 ) / 2);
    }
    tercon_echo_off();
    printf(ANSI_BLINK_SLOW "Press Enter to continue..." ANSI_BLINK_OFF);
    buffer_clear();
    tercon_echo_on();
}

void displayRoom() {

    struct Room room;
    FILE *fp;
    fp = fopen(roomsdb, "rb");

    Terminal term = tercon_init_rows_cols();
    displayRoomInfoLogo();

    int room_id; 
    printf("\x1b[%d;%dHEnter Room ID(%d-%d): ", 13, (term.columns - 21) / 2, 1, TOTAL_ROOMS);  
    room_id = getnuminput(5);
        
    if (room_id < 1 || room_id > TOTAL_ROOMS) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid Room ID.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 16) / 2);
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

    tercon_echo_off();
    printf(ANSI_BLINK_SLOW "\x1b[%d;%dHPress Enter to continue..." ANSI_BLINK_OFF, term.rows - 4, (term.columns - 26) / 2);
    buffer_clear();
    tercon_echo_on();
}

void displayAllRooms() {

    struct Room room;
    FILE *fp;
    fp = fopen(roomsdb, "rb");
    
    Terminal term = tercon_init_rows_cols();
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

    tercon_echo_off();
    printf(ANSI_BLINK_SLOW "\x1b[%dGPress Enter to continue..." ANSI_BLINK_OFF, (term.columns - 26) / 2);
    buffer_clear();
    tercon_echo_on();
}

void displayGuest() {

    struct Guest guest;
    FILE *fp;
    fp = fopen(guestsdb, "rb");

    Terminal term = tercon_init_rows_cols();
    displayGuestInfoLogo();

    int guest_id;
    int found = 0;
    
    printf("\x1b[%d;%dHEnter Guest ID: ", 13, (term.columns - 16) / 2);  
    guest_id = getnuminput(8);

    if (guest_id == -1) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input length.Must be maximum 8 chars long.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 50) / 2);  
    } else if (guest_id == 0 || guest_id == -2) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid Guest ID!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 17) / 2);
    } else if (guest_id == -3) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHNo input provided.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 17) / 2);
    } else {
        while(found == 0) {
            fread(&guest, sizeof(guest), 1, fp);
            if(feof(fp)) {
                break;            
            } else if(guest.id == guest_id && guest.id != 0) {
                displayGuestInfo(guest);
                found = 1;
            }
        }
    }
    fclose(fp);

    if(found == 0 && guest_id > 0) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHNo Guest found with the given ID.\n" ANSI_COLOR_RESET, term.rows - 2, (term.columns - 33) / 2);
        printf(ANSI_COLOR_RED "\x1b[%d;%dHDisplay all guests with option 5 of main menu to check the Guests IDs.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 70) / 2);
    }

    tercon_echo_off();
    printf(ANSI_BLINK_SLOW "\x1b[%d;%dHPress Enter to continue..." ANSI_BLINK_OFF, term.rows - 4, (term.columns - 26) / 2);
    buffer_clear();
    tercon_echo_on();
}

void displayAllGuests() {

    struct Guest guest;
    FILE *fp;
    fp = fopen(guestsdb, "rb");
    
    Terminal term = tercon_init_rows_cols();
    displayAllGuestsLogo();

    while(1) {
        fread(&guest, sizeof(guest), 1, fp);
        if(feof(fp)) {
            break;            
        } else if (guest.id != 0) {
            displayAllGuestsInfo(guest);
        }
    }
    fclose(fp);

    tercon_echo_off();
    printf(ANSI_BLINK_SLOW "\x1b[%d;%dHPress Enter to continue..." ANSI_BLINK_OFF, term.rows - 4, (term.columns - 26) / 2);
    buffer_clear();
    tercon_echo_on();
}

void modify() {

    Terminal term = tercon_init_rows_cols();

    while(1) {

        displayModifyLogo();
        int choice = getnuminput(4);
        // We need this error value here for handling errors and consume also the buffer when needed.
        int error = 0;
        // This is the following cases return value.It will be probably usefull so it is wise to save it to case_rv variable.
        int case_rv = 0;
        if (choice == -1) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
            error = 1;
        } else if (choice == -2) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
            error = 1;
        } else if (choice == -3) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
            error = 1;
        } else if ((choice > 2 && choice < 20) || choice > 20) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHThis number doesn't corresponds to a choice!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 44) / 2);
            error = 2;
        } else {
            // Switch case functions can be found with the given order in modify.c file.
            switch(choice) {
                case 1 :
                    case_rv = modifyRoom();
                    if (case_rv != 20) {
                        error = 2;
                    }
                    break;
                case 2 : modifyGuest();
                    break;
                case 20 :
                    return;
                default :
                    break;
            }
        }
        if (error != 0) {
            tercon_echo_off();
            printf(ANSI_BLINK_SLOW "\x1b[%d;%dH\x1b[2KPress Enter to continue..." ANSI_BLINK_OFF, term.rows - 4, (term.columns - 26) / 2);
            buffer_clear();
            if (error == 1) {
                getc(stdin);
            }
            tercon_echo_on();
        }
    }
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

    getc(stdin);
}

void deleteReservation() {

    struct Reservation res;
    FILE *fp;
    fp = fopen(reservationsdb, "rb");

    char c;
    while((c = getc(stdin) != '\n') && c != '\t');

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
    while((c = getc(stdin) != '\n') && c != '\t');

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
    while((c = getc(stdin) != '\n') && c != '\t');

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
    while((c = getc(stdin) != '\n') && c != '\t');

    printf("Enter which year you want to display: ");
    int input_year = getInteger(48, 5);

    if ((input_year < STARTING_YEAR && input_year != 0) || input_year > FINISHING_YEAR) {
        printf(ANSI_COLOR_RED "\nProvide a correct year.\nAre you sure year is in range of Software STARTING_YEAR - FINISHING_YEAR?\n\n" ANSI_COLOR_RESET);
    } else if(input_year >= STARTING_YEAR && input_year <= FINISHING_YEAR) {
        printf("Year: %d\nDisplaying Room Annually Reservations...\n\n", input_year);
        displayAllRoomsAnnuallyReservationsInfo(input_year);
    }

    printf("Press Enter to continue...");
}

void main_error() {

    char c;
    Terminal term = tercon_init_rows_cols();
    tercon_echo_off();

    printf(ANSI_COLOR_GREEN "\x1b[%d;%dHPress Enter to continue..." ANSI_COLOR_RESET, term.rows - 4, (term.columns - 28) / 2);
    printf("\x1b[%d;%dH          >>> ", 22, (term.columns - 51) / 2);

    while((c = getc(stdin) != '\n') && c != '\t');

    tercon_echo_on();
}

