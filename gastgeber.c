#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

struct Room {
    int id;
    char type[20];
    int guest_id;
    bool reserved;
};

void reserve();
void display();
void displayAll();
void modify();
void clearRes();
void search();
/* Function to keep the program runing */
char mygetch();

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
            printf("Room ID: %d\n", room.id);
            printf("Room Type: %s\n", room.type);
            printf("Guest ID: %d\n", room.guest_id);
            printf("Room Reserved: %s\n", room.reserved ? "true" : "false");
        }
        fclose(fp);
    }

    int choice;

    while(1) {
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

        scanf("%d", &choice);

        switch(choice) {
            case 1 : reserve();
                break;
            case 0 : exit(0);
                break;
            default :
                break;
        }
        getc(stdin);
    }
}

void reserve() {

    struct Room room;
    FILE *fp;
    fp = fopen(roomsdb, "rb");

    int room_id, found = 0;

    system("clear");
    printf("*************************************\n");
    printf("*       Hotel Room reservation.     *\n");
    printf("*************************************\n\n");
    
    while(1) {
        printf("Enter Room id:\n");  
        int test = scanf("%d", &room_id);
        
        if(test == 0) {
            printf("Invalid Room ID.\n");
            scanf("%*[^\n]");
            printf("Enter again a correct Room ID:\n");
            test = scanf("%d", &room_id);
        } else {
            while(1) {
                fread(&room, sizeof(room), 1, fp);
                if(feof(fp)) {
                    break;
                }
                if(room.id == room_id) {
                    found = 1;
                    printf("Room ID: %d\n", room.id);
                    printf("Room Type: %s\n", room.type);
                    printf("Guest ID: %d\n", room.guest_id);
                    printf("Room Reserved: %s\n", room.reserved ? "true" : "false");
                    break;
                }
            }
            if(found == 0) {
                printf("No Room found with the given ID.\n");
            }
        }
        getc(stdin);
    }
    fclose(fp);
    printf("Press Enter to continue...\n");

    int next = getc(stdin);
    printf("Next = %d", next);
}

