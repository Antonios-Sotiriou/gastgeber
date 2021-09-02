#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

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

void reserve();
void display();
void displayAll();
void modify();
void clearRes();
void search();
void displayMainLogo();
void displayRoomInfoLogo();

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
        while(1) {
            fread(&room, sizeof(room), 1, fp);
            if(feof(fp)) {
                break;
            }
            printf(" --------------------------------------------------------\n");
            printf("|  Room ID  |  Room Type  |  Guest ID  |  Room Reserved  |\n");
            printf(" --------------------------------------------------------\n");
            printf("|    %d     |    %s       |    %d      |       %s        |\n", room.id, room.type, room.guest_id, room.reserved ? "true" : "false");
            printf(" --------------------------------------------------------\n");
        }
        fclose(fp);
    }

    int choice;

    while(1) {

        displayMainLogo();
        scanf("%d", &choice);

        switch(choice) {
            case 1 : reserve();
                break;
            case 2 : display();
                break;
            case 0 : system("clear");
                exit(0);
                break;
            default :
                break;
        }
        getc(stdin);
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

    char room_id[63]; 
    int found = 0, check = 0;
    char c; 
    while(c = getc(stdin) != '\n' && c != '\t');
    
    while(check == 0) {
    
        printf("Enter Room ID(1-99):\n");  
        fgets(room_id, 63, stdin);
        
        if (strlen(room_id) < 1 || strlen(room_id) > 4) {
            check = 0;
            displayRoomInfoLogo();
            printf("Invalid Room ID.\n");
            continue;
        } else if (sscanf(room_id, "%d", &check) != 1) {
            check = 0;
            displayRoomInfoLogo();
            printf("Invalid Room ID.Provide a number.\n");
            continue;
        } else if (check < 1 || check > 100) {
            check = 0;
            displayRoomInfoLogo();
            printf("Room ID's range is 1 to 99.\n");
            continue;
        } else {
            while(1) {
                fread(&room, sizeof(room), 1, fp);
                if(feof(fp)) {
                    break;
                }
                if(room.id == check) {
                    printf(" ------------------------------------------------------------------------------\n");
                    printf("|Room ID            |Room Type          |Guest ID           |Room Reserved     |\n");
                    printf(" ------------------------------------------------------------------------------\n");
                    printf("|%d                 |%s                 |%d                 |%s                |\n", room.id, room.type, room.guest_id, room.reserved ? "true" : "false");
                    printf(" ------------------------------------------------------------------------------\n");
                }
            }
        }
    }
    fclose(fp);
    printf("Press Enter to continue...\n");
}

/* Display functions for logos and signs. */
void displayMainLogo() {
    //system("clear");
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

