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

int main(int argc, char *argv[]) {

    if(argc > 1 && strcmp(argv[1], "init") == 0) {
        createDaysDb();
        createRoomsDb();
        createReservationsDb();
        createGuestsDb();
        printf("argv: %s", argv[1]);
        getc(stdin);
    }

    int choice;
    char c;

    signal(SIGWINCH, (void*)&displayMainLogo);

    while(1) {

        char *positioning = displayMainLogo();
        printf("%s           >>> ", positioning);
        scanf("%2d", &choice);

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
            case 0 : clear_scr();
                exit(0);
                break;
            default : 
                break;
        }
        while((c = getc(stdin) != '\n') && c != '\t');
        free(positioning);
    }
    return 1;
}

void reserve() {

    struct Reservation reservation;
    FILE *fp;
    fp = fopen(reservationsdb, "ab");

    displayRoomReservationLogo();

    int room_id;
    int found = 0;
    // counter function which finds the last reservation record!
    int next_id = getNextReservationEntry();

    char c;
    while((c = getc(stdin) != '\n') && c != '\t');

    while(found == 0) {

        printf("Enter Room ID(%d-%d): ", 1, TOTAL_ROOMS);  
        room_id = getnuminput(5);
        
        if (room_id < 1 || room_id > TOTAL_ROOMS) {
            clear_scr();
            printf(ANSI_COLOR_RED "\nInvalid Room ID.\n" ANSI_COLOR_RESET);
            break;
        } else {
            reservation.id = next_id;
            reservation.room.id = room_id;
            // Check if guest already exists,if exists he must be marked as Repeated Guest else if not create after success reservation.
            reservation.guest = handleGuest();

            int from_date = 0;
            while(from_date == 0) {
                printf("Room reserve from date: \n");
                if(getformatedDate(reservation.from_date) == 1 && checkFromDate(reservation) == 0) {
                    from_date = 1;
                } else {
                    printf("\nPress Enter to continue...\n");
                    return;
                }
            }
            int to_date = 0;
            while(to_date == 0) {
                printf("To date: \n");
                if(getformatedDate(reservation.to_date) == 1 && compareDates(reservation.from_date, reservation.to_date) == 1) {
                    if(checkAllDates(reservation) == 0) {
                        printf(ANSI_COLOR_GREEN "Room Succesfully reserved!\n" ANSI_COLOR_RESET);
                        // Write also the Guest to guests database.
                        createGuestEntry(reservation);
                        to_date = 1;
                        fwrite(&reservation, sizeof(reservation), 1, fp);
                    } else {
                        break;
                    }
                }
            }
            found = 1;
        }
    }
    fclose(fp);

    printf("\nPress Enter to continue...\n");
}

void displayRoom() {

    struct Room room;
    FILE *fp;
    fp = fopen(roomsdb, "rb");

    displayRoomInfoLogo();

    int room_id; 
    char c; 
    while((c = getc(stdin) != '\n') && c != '\t');
    
    printf("Enter Room ID(%d-%d): ", 1, TOTAL_ROOMS);  
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
    while((c = getc(stdin) != '\n') && c != '\t');
    
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

void displayGuest() {

    struct Guest guest;
    FILE *fp;
    fp = fopen(guestsdb, "rb");

    displayGuestInfoLogo();

    int guest_id;
    int found = 0;
    char c; 
    while((c = getc(stdin) != '\n') && c != '\t');
    
    printf("Enter Guest ID:\n");  
    guest_id = getnuminput(8);
        
    if (guest_id <= 0) {
        printf(ANSI_COLOR_RED "\nInvalid Guest ID.\n" ANSI_COLOR_RESET);
    } else {
        while(found == 0) {
            fread(&guest, sizeof(guest), 1, fp);
            if(feof(fp)) {
                break;            
            } else if(guest.id == guest_id) {
                displayGuestInfo(guest);
                found = 1;
            }
        }
    }
    fclose(fp);
    if(found == 0) {
        printf("No Guest found with the given ID.\nDisplay all guests with option 5 of main menu to check the Guests IDs.\n");
    }

    printf("\nPress Enter to continue...\n");  
}

void displayAllGuests() {

    struct Guest guest;
    FILE *fp;
    fp = fopen(guestsdb, "rb");

    char c; 
    while((c = getc(stdin) != '\n') && c != '\t');
    
    displayAllGuestsLogo();

    while(1) {
        fread(&guest, sizeof(guest), 1, fp);
        if(feof(fp)) {
            break;            
        } else {
            displayAllGuestsInfo(guest);
        }
    }
    fclose(fp);

    printf("\nPress Enter to continue...\n");
}

void modify() {
    
    displayModifyLogo();
    
    int choice;
    scanf("%d", &choice);

    switch(choice) {
        case 1 : modifyRoom();
            break;
        case 2 : modifyGuest();
            break;
        case 0 : clear_scr();
            break;
        default :
            break;
    }

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

