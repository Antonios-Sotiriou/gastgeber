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
/*********************
 * Global constants 
 ********************/
#include "header_files/global/global_vars.h"
#include "header_files/global/res_db_path.h"
#include "header_files/global/rooms_db_path.h"
#include "header_files/global/days_db_path.h"
#include "header_files/global/guests_db_path.h"
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

#include "header_files/userinput.h"
#include "header_files/display.h"
#include "header_files/comparedates.h"
#include "header_files/init_dbs.h"
#include "header_files/reserve.h"
#include "header_files/delete_reservation.h"
#include "header_files/modify.h"
#include "header_files/handle_guest.h"
#include "header_files/joins.h"

/* functionality functions */
void gastgeber();
void reserve();
void displayRoom();
void displayAllRooms();
void displayGuest();
void displayAllGuests();
void modify();
void displayReservations();
void displayReservationsByDate();
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
        printf(ANSI_BLINK_SLOW "\x1b[2KPress Enter to continue..." ANSI_BLINK_OFF);
        buffer_clear();
    }
    gastgeber();

    return 1;
}

void gastgeber() {

    int choice;
    while(1) {

        Terminal term = tercon_init_rows_cols();
        // All the functions that start with the word [ display ] and are not part of the following
        // [ switch ],can be found in display.c file.
        displayMainLogo();
        // Can be found in userinput.c
        choice = getnuminput(4, false);
        int error = 0;
        if ((choice > 12 && choice < 20) || choice > 20) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHThis number doesn't corresponds to a choice!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 44) / 2);
            error = 1;
        }else if (choice == -1) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
            error = 2;
        } else if (choice == -2) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
            error = 3;
        } else if (choice == -3) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
            error = 4;
        } else {
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
                case 8 : displayReservationsByDate();
                    break;
                case 9 : deleteReservation();
                    break;
                case 10 : displayAnnuallyAvailabillity();
                    break;
                case 11 : displayRoomAnnuallyReservations();
                    break;
                case 12 : displayAllRoomsAnnuallyReservations();
                    break;
                case 20 : clear_scr();
                    exit(0);
                    break;
                default :
                    break;
            }
        }

        tercon_echo_off();
        if (error) {
            if (error > 1) {
                buffer_clear();
            }
            printf(ANSI_BLINK_SLOW "\x1b[%d;%dH\x1b[2KPress Enter to continue..." ANSI_BLINK_OFF, term.rows - 4, (term.columns - 26) / 2);
            buffer_clear();
        }
        tercon_echo_on();
    }
}

void reserve() {
    /****************************************************************
    * The function which handles the reservations.Checks the dates
    * and room availability, writes to the databases the guest
    * and everythink else if they meet the requirements.
    *         Needs to be simplyfied in the future!!!
    ****************************************************************/
    struct Reservation reservation;
    FILE *fp;
    // Setting dynamically the absolut path.
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, reservationsdb);
    fp = fopen(abs_path, "ab");

    Terminal term = displayRoomReservationLogo();
    term.cursor_x = 0;
    term.cursor_y = 13;
    
    int num_of_days = 0;
    int room_id;
    int found = 0;
    int error = 0;
    // counter function which finds the last reservation record! Can be found in init_dbs.c file
    int next_id = getNextReservationEntry();

    while(found == 0) {

        printf("\x1b[%d;%dH  Enter Room ID(%d-%d): ", term.cursor_y, (term.columns - 51) / 2, 1, TOTAL_ROOMS); 
        term.cursor_y += 1;
        room_id = getnuminput(5, false);

        if ((room_id < 1 && room_id >= 0) || room_id > TOTAL_ROOMS) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid Room ID.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 16) / 2);
            error = 1;
            break;
        } else if (room_id == -1) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHWrong format.Check for letters.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 31) / 2);
            error = 2;
            break;
        } else if (room_id == -2) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.Check for special charackters or spaces.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 56) / 2);
            error = 3;
            break;
        } else if (room_id == -3) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid number length.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
            error = 4;
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
                    tercon_move_y_x(16, (term.columns - 51 ) / 2);
                    term.cursor_y = 16;
                } else {
                    tercon_move_y_x(17, (term.columns - 51 ) / 2);
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
                        tercon_clear_error_log(reservation);
                        tercon_move_y_x(term.rows - 4, (term.columns - 26 ) / 2);
                        printf("\x1b[2K");
                        buffer_clear();
                        continue;
                    } else if (c == '\n' || c == '\t') {
                        tercon_clear_lines(term.rows - 4, term.rows - 5);
                        tercon_clear_error_log();
                        continue;
                    } else {
                        fclose(fp);
                        return;
                    }
                }
            }
            int to_date = 0;
            // repeated here identifies in the user tried to supplied to_date more than 1 time.If so we take actions accordingly.
            bool repeated = false;
            while(to_date == 0) {
                if (!repeated) {
                    // User goes for first time throught the function.Print < To date > prompt.If user comes again
                    // through the function the displayPreviousStep() function is responsible to handle the output.
                    tercon_move_y_x(term.cursor_y + 1, (term.columns - 51 ) / 2);
                    printf(ANSI_ERASE_LINE "  To date: ");
                }
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
                    } else if (num_of_days == -2) {
                        // this else clause means that the user canceled the apply of the reservation so..
                        // there is no need to ask if he wants to retry.Skip the next if dialogue by breaking out of the loop.
                        to_date = 1;
                    } else {
                        step_count = 1;
                    }
                } else {
                    step_count = 1;
                }
                // Steps are importand to set correct the cursor and prompt for retry.
                if (step_count != 0) {
                    if (num_of_days > 2) {
                        tercon_move_y_x(term.rows + num_of_days, (term.columns - 26 ) / 2);
                    } else {
                        tercon_move_y_x(term.rows - 4, (term.columns - 26 ) / 2);
                    }
                    printf(ANSI_ERASE_LINE "Do you want to retry? [Y/N]: ");
                    char c = getc(stdin);
                    if (c == 'y' || c == 'Y') {
                        if (num_of_days > 2) {
                            // With this function rerender whole last screen and return to to_date prompt.We need this
                            // function here to avoid terminal mess when the already booked days are alot. 
                            displayPreviousStep(reservation);
                            num_of_days = 0;
                            repeated = true;
                            buffer_clear();
                        } else {
                            tercon_clear_error_log();
                            tercon_move_y_x(term.rows - 4, (term.columns - 26 ) / 2);
                            printf("\x1b[2K");
                            repeated = false;
                            buffer_clear();
                        }
                        continue;
                    } else if (c == '\n' || c == '\t') {
                        tercon_clear_lines(term.rows - 4, term.rows - 5);
                        tercon_clear_error_log();
                        continue;
                    } else {
                        fclose(fp);
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
    printf(ANSI_BLINK_SLOW "\x1b[2KPress Enter to continue..." ANSI_BLINK_OFF);
    if (error && error > 1) {
        buffer_clear();
    }
    buffer_clear();
    tercon_echo_on();
}

void displayRoom() {

    struct Room room;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, roomsdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate roomsdb file displayRoom()");
        exit(127);
    }

    Terminal term = tercon_init_rows_cols();
    displayRoomInfoLogo();

    int room_id; 
    int error = 0;
    printf("\x1b[%d;%dHEnter Room ID(%d-%d): ", 13, (term.columns - 21) / 2, 1, TOTAL_ROOMS);  
    room_id = getnuminput(5, true);
        
    if ((room_id < 1 && room_id >= 0) || room_id > TOTAL_ROOMS) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid Room ID.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 16) / 2);
        error = 1;
    } else if (room_id == -1) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHWrong format.Check for letters.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 31) / 2);
        error = 2;
    } else if (room_id == -2) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.Check for special charackters or spaces.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 56) / 2);
        error = 3;
    } else if (room_id == -3) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid number length.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
        error = 4;
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
    if (error && error > 1) {
        buffer_clear();
    }
    buffer_clear();
    tercon_echo_on();
}

void displayAllRooms() {

    struct Room room;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, roomsdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate roomsdb file displayAllRooms()");
        exit(127);
    }
    
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
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, guestsdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate guestsdb file displayGuest()");
        exit(127);
    }

    Terminal term = tercon_init_rows_cols();
    displayGuestInfoLogo();

    int guest_id;
    int found = 0;
    int error = 0;
    
    printf("\x1b[%d;%dHEnter Guest ID: ", 13, (term.columns - 16) / 2);  
    guest_id = getnuminput(8, true);

    if (guest_id == -1) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHWrong format.Check for letters.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 31) / 2);
        error = 1;        
    } else if (guest_id == -2) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.Check for special charackters or spaces.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 56) / 2);
        error = 2;       
    } else if (guest_id == -3) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid number length.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
        error = 3;
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

    if(found == 0 && guest_id >= 0) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHNo Guest found with the given ID.\n" ANSI_COLOR_RESET, term.rows - 2, (term.columns - 33) / 2);
        printf(ANSI_COLOR_RED "\x1b[%d;%dHDisplay all guests with option 5 of main menu to check the Guests IDs.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 70) / 2);
    }

    tercon_echo_off();
    printf(ANSI_BLINK_SLOW "\x1b[%d;%dHPress Enter to continue..." ANSI_BLINK_OFF, term.rows - 4, (term.columns - 26) / 2);
    if (error) {
        buffer_clear();
    }
    buffer_clear();
    tercon_echo_on();
}

void displayAllGuests() {

    struct Guest guest;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, guestsdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate guestsdb file displayAllGuests()");
        exit(127);
    }
    
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
    printf(ANSI_BLINK_SLOW "\x1b[%dGPress Enter to continue..." ANSI_BLINK_OFF, (term.columns - 26) / 2);
    buffer_clear();
    tercon_echo_on();
}

void modify() {

    while(1) {
        Terminal term = tercon_init_rows_cols();
        displayModifyLogo();
        int choice = getnuminput(4, true);
        // We need this error value here for handling errors and consume also the buffer when needed.
        int error = 0;
        // This is the following cases return value.It will be probably usefull so it is wise to save it to case_rv variable.
        int case_rv = 0;
        if ((choice > 2 && choice < 20) || choice > 20) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHThis number doesn't corresponds to a choice!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 44) / 2);
            error = 1;
        }else if (choice == -1) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
            error = 2;
        } else if (choice == -2) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
            error = 3;
        } else if (choice == -3) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
            error = 4;
        } else {
            // Switch case functions can be found with the given order in modify.c file.
            switch(choice) {
                case 1 :
                    case_rv = modifyRoom();
                    if (case_rv != 20 && case_rv != 0) {
                        error = case_rv;
                    } else if (case_rv == 0) {
                        error = 2;
                    }
                    break;
                case 2 : 
                    case_rv = modifyGuest();
                    if (case_rv != 20 && case_rv != 0) {
                        error = case_rv;
                    } else if (case_rv == 0) {
                        error = 2;
                    }
                    break;
                case 20 :
                    return;
                default :
                    break;
            }
        }
        if (error) {
            tercon_echo_off();
            printf(ANSI_BLINK_SLOW "\x1b[%d;%dH\x1b[2KPress Enter to continue..." ANSI_BLINK_OFF, term.rows - 4, (term.columns - 26) / 2);
            if (error > 1) {
                buffer_clear();
            }
            buffer_clear();
            tercon_echo_on();
        }
    }
}

void displayReservations() {

    struct Reservation res;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, reservationsdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate reservationsdb file displayReservations()");
        exit(127);
    }

    Terminal term = tercon_init_rows_cols();
    displayAllReservationsLogo();

    while(1) {
        fread(&res, sizeof(res), 1, fp);
        if(feof(fp)) {
            break;            
        } else if (res.id != 0) {
            displayAllReservationsInfo(res);
        }
    }
    fclose(fp);

    tercon_echo_off();
    printf(ANSI_BLINK_SLOW "\x1b[%dGPress Enter to continue..." ANSI_BLINK_OFF, (term.columns - 26) / 2);
    buffer_clear();
    tercon_echo_on();
}

void displayReservationsByDate() {

    struct Day day, request;
    Terminal term = tercon_init_rows_cols();

    displayReservationsByDateLogo();
    int found = 0;
    int error = 0;
    while (found == 0) {
        printf("\x1b[%d;%dH\x1b[2KEnter date: ", 13, (term.columns - 12) / 2);
        if (getformatedDate(request.date) == 1) {
            FILE *fp;
            char abs_path[PATH_LENGTH];
            joinHome(abs_path, daysdb);
            fp = fopen(abs_path, "rb");
            if (fp == NULL) {
                perror("Could not locate daysdb file displayReservationsByDate()");
                exit(127);
            }
            while (1) {
                fread(&day, sizeof(day), 1, fp);
                if (feof(fp)) {
                    break;
                } else if (strcmp(day.date, request.date) == 0) {
                    tercon_clear_lines(14, 13);
                    printf(ANSI_MOVE_CURSOR_COL "Displaying Reservations for date: %s", (term.columns - 44) / 2, request.date);
                    found = 1;
                    for (int i = 1; i <= (sizeof(day.res_ids) / sizeof(int)) - 1; i++) {
                        if (day.res_ids[i] < 1000000) {
                            printf("Rooms of date %s: %d\n", day.date, day.res_ids[i]);
                        }
                    }
                    break;
                } else {
                    found = -1;
                }
            }
            fclose(fp);
        }
        if (found <= 0) {
            if (found == -1) {
                printf(ANSI_COLOR_RED "\x1b[%d;%dHThis Day doesn't exist!" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 23) / 2);
            }
            printf("\x1b[%d;%dHWould you like to try again?: [Y/N]. ", term.rows - 4, (term.columns - 37) / 2);
            char c = getc(stdin);
            if(c == 'Y' || c == 'y') {
                printf("\x1b[%d;%dH\x1b[2K", term.rows - 4, (term.columns - 37) / 2);
                tercon_clear_error_log();
                found = 0;
                getc(stdin);
                continue;
            } else if (c == '\n' || c == '\t') {
                error = 6;
                break;
            } else {
                error = 5;
                break;
            }
        }
    }
    tercon_echo_off();
    if (!error) {
        printf(ANSI_BLINK_SLOW "\x1b[%d;%dH\x1b[2KPress Enter to continue..." ANSI_BLINK_OFF, term.rows - 4, (term.columns - 26) / 2);
        buffer_clear();
    } else if (error && error < 6) {
        buffer_clear();
    }
    tercon_echo_on();
}

void deleteReservation() {

    struct Reservation res;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, reservationsdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate reservationsdb file deleteReservation()");
        exit(127);
    }

    Terminal term = tercon_init_rows_cols();
    displayDeleteResLogo();
    printf(ANSI_MOVE_CURSOR_COL "Enter Reservation's id you want to delete: ", (term.columns - 43) / 2);
    int res_id = getnuminput(6, true);
    int found = 0;
    int error= 0;

    if (res_id == 0) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHThere is no Reservation with 0 ID.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 37) / 2);
        found = 2;
        error = 1;
    } else if (res_id == -1) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHWrong format.Check for letters.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 31) / 2);
        found = 2;
        error = 2;
    } else if (res_id == -2) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.Check for special charackters or spaces.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 56) / 2);
        found = 2;
        error = 3;
    } else if (res_id == -3) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid number length!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
        found = 2;
        error = 4;
    }
    while(found == 0) {
        fread(&res, sizeof(res), 1, fp);
        if(feof(fp)) {
            break;
        } else if(res.id == res_id) {
            displayReservationInfo(res);
            printf(ANSI_COLOR_RED "\x1b[%d;%dHAre you sure you want to delete the Reservation: [Y/N]? " ANSI_COLOR_RESET, term.rows - 4, (term.columns - 56) / 2);
            char c = getc(stdin);
            if(c == 'Y' || c == 'y') {
                // Following function can be found in delete_reservation.c file.This function is responsible
                // to start the delete reservation functions familly.It collects reservation's dates and deletes room from dates.
                getResDatesToDelete(res);
                getc(stdin);
                found = 1;
            } else if (c == '\n' || c == '\t') {
                printf(ANSI_COLOR_RED "\x1b[%d;%dHReservation deletion canceled." ANSI_COLOR_RESET, term.rows - 1, (term.columns - 30) / 2);
                found = 2;
            } else {
                printf(ANSI_COLOR_RED "\x1b[%d;%dHReservation deletion canceled." ANSI_COLOR_RESET, term.rows - 1, (term.columns - 30) / 2);
                getc(stdin);
                found = 2;
            }
        }
    }
    fclose(fp);
    if(found == 0 && res_id > 0) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHNo Reservation found with this ID: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 37) / 2, res_id);
    } else if(found == 1) {
        // Following function can be found in delete_reservation.c file.It deletes the reservation.
        applyReservationDelete(res_id);
        printf(ANSI_COLOR_GREEN "\x1b[%d;%dHReservation has been deleted.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 29) / 2);
    }

    tercon_echo_off();
    printf(ANSI_BLINK_SLOW "\x1b[%d;%dH\x1b[2KPress Enter to continue..." ANSI_BLINK_OFF, term.rows - 4, (term.columns - 26) / 2);
    buffer_clear();
    if (error && error > 1) {
        buffer_clear();
    }
    tercon_echo_on();
}

void displayAnnuallyAvailabillity() {

    struct Day day, *st_arr;
    st_arr = malloc(sizeof(struct Day));
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, daysdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate daysdb file displayAnnuallyAvailabillity()");
        exit(127);
    }

    Terminal term = tercon_init_rows_cols();
    int input_year;
    char str_year[5];

    displayAnnuallyAvailabillityLogo();
    printf(ANSI_MOVE_CURSOR_TO "Which year would you like to display?: ", 13, (term.columns - 39) / 2);

    input_year = getnuminput(6, true);
    int i = 0;
    int dynamic_inc = 1;
    int error = 0;

    if((input_year < STARTING_YEAR && input_year >= 0) || input_year > FINISHING_YEAR) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHYear out of range: %d" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 21) / 2, input_year);
        error = 1;
    } else if (input_year == -1) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHWrong format.Check for letters.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 31) / 2);
        error = 2;
    } else if (input_year == -2) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.Check for special charackters or spaces.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 56) / 2);
        error = 3;
    } else if (input_year == -3) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid number length!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
        error = 4;
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
    }
    fclose(fp);

    tercon_echo_off();
    if (error) {
        printf(ANSI_BLINK_SLOW "\x1b[%d;%dHPress Enter to continue..." ANSI_BLINK_OFF, term.rows - 4, (term.columns - 26) / 2);
        if (error > 1) {
            buffer_clear();
        }
    } else {
        // Process and display.
        tercon_clear_lines(term.rows, 14);
        displayRoomsPerDay(st_arr, i);
        printf(ANSI_BLINK_SLOW "\x1b[%dGPress Enter to continue..." ANSI_BLINK_OFF, (term.columns - 26) / 2);
    }
    buffer_clear();
    tercon_echo_on();
}

void displayRoomAnnuallyReservations() {

    Terminal term = tercon_init_rows_cols();
    displayRoomAnnuallyReservationsLogo();

    printf(ANSI_MOVE_CURSOR_TO "Enter room ID you want to display: ", 13, (term.columns - 36) / 2);
    int room_id = getnuminput(6, false);
    int error = 0;
    if(room_id == 0 || room_id > TOTAL_ROOMS) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHProvide a correct room ID.\n" ANSI_COLOR_RESET, term.rows - 2, (term.columns - 26) / 2);
        printf(ANSI_COLOR_RED "\x1b[%d;%dHYou can check the room IDs from option 3 in main menu.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 54) / 2);
        error = 1;
    } else if (room_id == -1) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHWrong format.Check for letters.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 31) / 2);
        error = 2;
    } else if (room_id == -2) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.Check for special charackters or spaces.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 56) / 2);
        error = 3;
    } else if (room_id == -3) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid number length!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
        error = 4;
    } else {
        printf(ANSI_MOVE_CURSOR_TO "Enter which year you want to display: ", 14, (term.columns - 38) / 2);
        int input_year = getnuminput(6, false);
        if ((input_year < STARTING_YEAR && input_year >= 0) || input_year > FINISHING_YEAR) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHProvide a correct year.\n" ANSI_COLOR_RESET, term.rows - 2, (term.columns - 23) / 2);
            printf(ANSI_COLOR_RED "\x1b[%d;%dHAre you sure year is in range STARTING_YEAR - FINISHING_YEAR?\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 61) / 2);
            error = 1;
        } else if (input_year == -1) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHWrong format.Check for letters.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 31) / 2);
            error = 2;
        } else if (input_year == -2) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.Check for special charackters or spaces.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 56) / 2);
            error = 3;
        } else if (input_year == -3) {
            printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid number length!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
            error = 4;
        } else if(input_year >= STARTING_YEAR && input_year <= FINISHING_YEAR) {
            tercon_clear_lines(14, 13);
            displayRoomAnnuallyReservationsInfo(room_id, input_year);
        }
    }

    tercon_echo_off();
    printf(ANSI_BLINK_SLOW "\x1b[%d;%dH\x1b[2KPress Enter to continue..." ANSI_BLINK_OFF, term.rows - 4, (term.columns - 26) / 2);
    buffer_clear();
    if (error && error > 1) {
        buffer_clear();
    }
    tercon_echo_on();
}

void displayAllRoomsAnnuallyReservations() {

    struct Room room, rooms_arr[TOTAL_ROOMS - 1];
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, roomsdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate file displayAllRoomsAnnuallyReservationsInfo()");
        exit(127);
    }

    Terminal term = tercon_init_rows_cols();
    displayAllRoomsAnnuallyReservationsLogo();

    printf(ANSI_MOVE_CURSOR_TO "Enter which year you want to display: ", 13, (term.columns - 38) / 2);
    int input_year = getnuminput(6, false);
    int error = 0;
    int displaying = 0;

    if ((input_year < STARTING_YEAR && input_year >= 0) || input_year > FINISHING_YEAR) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHProvide a correct year.\n" ANSI_COLOR_RESET, term.rows - 2, (term.columns - 23) / 2);
        printf(ANSI_COLOR_RED "\x1b[%d;%dHAre you sure year is in range of STARTING_YEAR - FINISHING_YEAR?\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 64) / 2);
        error = 1;
    } else if (input_year == -1) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHWrong format.Check for letters.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 31) / 2);
        error = 2;
    } else if (input_year == -2) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.Check for special charackters or spaces.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 56) / 2);
        error = 3;
    } else if (input_year == -3) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid number length!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
        error = 4;
    } else if(input_year >= STARTING_YEAR && input_year <= FINISHING_YEAR) {
        int index = 0;
        while(1) {
            fread(&room, sizeof(room), 1, fp);
            if(feof(fp)) {
                break;
            } else {
                rooms_arr[index] = room;
                index++;
            }
        }
        fclose(fp);
        displaying = 1;
        displayAllRoomsAnnuallyReservationsInfo(rooms_arr, input_year);
    }

    tercon_echo_off();
    if (displaying) {
        printf(ANSI_BLINK_SLOW "\x1b[%dG\x1b[2KPress Enter to continue..." ANSI_BLINK_OFF, (term.columns - 26) / 2);
    } else {
        printf(ANSI_BLINK_SLOW "\x1b[%d;%dH\x1b[2KPress Enter to continue..." ANSI_BLINK_OFF, term.rows - 4, (term.columns - 26) / 2);
    }
    buffer_clear();
    if (error) {
        fclose(fp);
        if (error > 1) {
            buffer_clear();
        }
    }
    tercon_echo_on();
}

void main_error() {

    Terminal term = tercon_init_rows_cols();
    tercon_echo_off();

    printf(ANSI_COLOR_GREEN "\x1b[%d;%dHPress Enter to continue..." ANSI_COLOR_RESET, term.rows - 4, (term.columns - 28) / 2);
    printf("\x1b[%d;%dH          >>> ", 22, (term.columns - 51) / 2);

    buffer_clear();
    tercon_echo_on();
}

