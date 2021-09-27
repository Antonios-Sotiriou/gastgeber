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
#include "input_functions/userInputFunctions.h"
#include "compare_functions/compareFunctions.h"
#include "structures/room.h"
#include "structures/guest.h"
#include "structures/day.h"
#include "structures/reservations.h"
#include "header_files/paths.h"
#include "init_dbs/init_databases.h"
#include "reserve_functions/reserveAvailabillity.h"

/* functionality functions */
void reserve();
void display();
void displayAll();
void modify();
void displayReservations();
void clearRes();
void search();
/* Display functions */
void displayMainLogo();
void displayRoomReservationLogo();
void displayRoomInfoLogo();
void displayRoomInfo(struct Room room);
void displayAllRoomsLogo();
void displayAllRooms(struct Room room);
void displayAllReservationsLogo();
void displayAllReservations(struct Reservation reservation);
void displayInt(int id);
void displayStr(char *str);

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
            case 2 : display();
                break;
            case 3 : displayAll();
                break;
            case 6 : displayReservations();
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
        room_id = getnuminput();
        
        if (room_id < 1 || room_id > 100) {
            printf("Invalid Room ID.\n");
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
                        printf("Room Succesfully reserved!\n");
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

void display() {

    struct Room room;
    FILE *fp;
    fp = fopen(roomsdb, "rb");

    displayRoomInfoLogo();

    int room_id; 
    int found = 0;
    char c; 
    while(c = getc(stdin) != '\n' && c != '\t');
    
    while(found == 0) {
    
        printf("Enter Room ID(1-99):\n");  
        room_id = getnuminput();
        
        if (room_id < 1 || room_id > 100) {
            printf("Invalid Room ID.\n");
            continue;
        } else {
            while(1) {
                fread(&room, sizeof(room), 1, fp);
                if(feof(fp)) {
                    break;            
                } else if(room.id == room_id) {
                    displayRoomInfo(room);
                    found = 1;
                }
            }
        }
    }
    fclose(fp);
    printf("Press Enter to continue...\n");
}

void displayAll() {

    struct Room room;
    FILE *fp;
    fp = fopen(roomsdb, "rb");
    
    displayAllRoomsLogo();
    
    while(1) {
        fread(&room, sizeof(room), 1, fp);
        if(feof(fp)) {
            break;            
        } else {
            displayAllRooms(room);
        }
    }
    fclose(fp);
    printf("Press Enter to continue...\n");

    char c = getc(stdin);
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
            displayAllReservations(res);
        }
    }
    fclose(fp);
    printf("Press Enter to continue...\n");

    char c = getc(stdin);
}

/* Display functions for logos and signs. */
void displayMainLogo() {
    system("clear");
    printf("*****************************************************\n");
    printf("*                  Welcome to Vesta                 *\n");
    printf("*       The most Advanced and client oriented       *\n");
    printf("*             Hotel Managment Software              *\n");
    printf("*                                                   *\n");
    printf("*****************************************************\n\n");
    printf("Choose your action...\n\n");
    printf("1. Reserve a room\n");
    printf("2. Display room info\n");
    printf("3. Display all rooms\n");
    printf("4. Modify a room\n");
    printf("6. Display all Reservations\n");
    printf("7. Clear room Reservation\n");
    printf("8. Search\n");
    printf("0. Exit\n\n");
}
void displayRoomReservationLogo() {
    system("clear");
    printf("*************************************\n");
    printf("*      Room Reservation Panel.      *\n");
    printf("*************************************\n\n");   
}
void displayRoomInfoLogo() {
    system("clear");
    printf("*************************************\n");
    printf("*    Display Room Informations.     *\n");
    printf("*************************************\n\n");
}
void displayRoomInfo(struct Room room) {
    printf(" -----------------------------------------------------------------------------------------------------------------------------\n");
    printf("|      Room ID       |     Room Type      |      Guest ID      |    Room Reserved   |     From Date      |      To Date       |\n");
    printf(" -----------------------------------------------------------------------------------------------------------------------------\n");
    printf("|");
    displayInt(room.id);
    printf("|");
    displayStr(room.type);
    printf("|");
    displayInt(room.guest_id);
    printf("|");
    displayStr(room.reserved ? "true" : "false");
    printf("|");
    displayStr(room.from_date);
    printf("|");
    displayStr(room.to_date);
    printf("|\n");
    printf("------------------------------------------------------------------------------------------------------------------------------\n");
}
void displayAllRoomsLogo() {
    system("clear");
    printf("*************************************\n");
    printf("*  Display All Rooms Informations.  *\n");
    printf("*************************************\n\n");
    printf(" -----------------------------------------------------------------------------------------------------------------------------\n");
    printf("|      Room ID       |     Room Type      |      Guest ID      |    Room Reserved   |     From Date      |      To Date       |\n");
    printf(" -----------------------------------------------------------------------------------------------------------------------------\n");
}
void displayAllRooms(struct Room room) {
    printf("|");
    displayInt(room.id);
    printf("|");
    displayStr(room.type);
    printf("|");
    displayInt(room.guest_id);
    printf("|");
    displayStr(room.reserved ? "true" : "false");
    printf("|");
    displayStr(room.from_date);
    printf("|");
    displayStr(room.to_date);
    printf("|\n");
    printf("------------------------------------------------------------------------------------------------------------------------------\n");
}
void displayAllReservationsLogo() {
    system("clear");
    printf("*************************************\n");
    printf("*      Display All Reservations.    *\n");
    printf("*************************************\n\n");
    printf(" --------------------------------------------------------------------------------------------------------\n");
    printf("|   Reservation ID   |      Room Id       |      Guest ID      |     From Date      |      To Date       |\n");
    printf(" --------------------------------------------------------------------------------------------------------\n");
}
void displayAllReservations(struct Reservation reservation) {
    printf("|");
    displayInt(reservation.id);
    printf("|");
    displayInt(reservation.room.id);
    printf("|");
    displayInt(reservation.guest.id);
    printf("|");
    displayStr(reservation.from_date);
    printf("|");
    displayStr(reservation.to_date);
    printf("|\n");
    printf("---------------------------------------------------------------------------------------------------------\n");
}
void displayInt(int id) {

    char converted[30];

    sprintf(converted, "%d", id);
    int d = (20 - strlen(converted)) / 2;

    for(int i = 0; i <= 20 - strlen(converted); i++) {
        if(i < d) {
            printf(" ");
        } else if(i > d) {
            printf(" ");
        } else {
            printf("%s", converted);
        }
    }
}
void displayStr(char *str) {

    int d = (20 - strlen(str)) / 2;

    for(int i = 0; i <= 20 - strlen(str); i++) {
        if(i < d) {
            printf(" ");
        } else if(i > d) {
            printf(" ");
        } else {
            printf("%s", str);
        }
    }
}


