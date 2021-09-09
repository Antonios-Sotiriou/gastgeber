#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "input_functions/user_input_experimental.c"

struct Room {
    int id;
    char type[20];
    int guest_id;
    bool reserved;
};
struct Customer {
    int id;
    char first_name[30];
    char last_name[30];
    char nationality[30];
    bool stammgast;
};
/* functionality functions */
void reserve();
void display();
void displayAll();
void modify();
void clearRes();
void search();
/* Display functions */
void displayMainLogo();
void displayRoomInfoLogo();
void displayRoomInfo(struct Room room);
void displayAllRoomsLogo();
void displayAllRooms(struct Room room);
void displayInt(int id);
void displayStr(char *str);

char roomsdb[] = {"/home/as/gastgeber/data/roomsdb.dat"};

void main() {

    FILE *fp;
    fp = fopen(roomsdb, "rb");
    struct Room room;

    if(fp == NULL) {
        fp = fopen(roomsdb, "wb");
        for(int i = 1; i <= 100; i++) {
            room.id = i;
            room.guest_id = 0;
            room.reserved = false;
            fwrite(&room, sizeof(room), 1, fp);
        }
        fclose(fp);
    } else {
        displayAllRoomsLogo();
        while(1) {
            fread(&room, sizeof(room), 1, fp);
            if(feof(fp)) {
                break;
            }
            displayAllRooms(room);
        }
        fclose(fp);
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
            case 0 : system("clear");
                exit(0);
                break;
            default :
                break;
        }
        //getc(stdin);
        while(c = getc(stdin) != '\n' && c != '\t');
    }
}

void reserve() {
    system("clear");
    printf("Reserve room...\n");
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
    printf("5. Clear room Reservation\n");
    printf("6. Search\n");
    printf("0. Exit\n\n");
}
void displayRoomInfoLogo() {
        system("clear");
        printf("*************************************\n");
        printf("*    Display Room Informations.     *\n");
        printf("*************************************\n\n");
}
void displayRoomInfo(struct Room room) {
    printf(" -----------------------------------------------------------------------------------\n");
    printf("|      Room ID       |     Room Type      |      Guest ID      |    Room Reserved   |\n");
    printf(" -----------------------------------------------------------------------------------\n");
    printf("|");
    displayInt(room.id);
    printf("|");
    displayStr(room.type);
    printf("|");
    displayInt(room.guest_id);
    printf("|");
    displayStr(room.reserved ? "true" : "false");
    printf("|\n");
    printf("------------------------------------------------------------------------------------\n");
}
void displayAllRoomsLogo() {
        system("clear");
        printf("*************************************\n");
        printf("*  Display All Rooms Informations.  *\n");
        printf("*************************************\n\n");
        printf(" -----------------------------------------------------------------------------------\n");
        printf("|      Room ID       |     Room Type      |      Guest ID      |    Room Reserved   |\n");
        printf(" -----------------------------------------------------------------------------------\n");
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
    printf("|\n");
    printf("------------------------------------------------------------------------------------\n");
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