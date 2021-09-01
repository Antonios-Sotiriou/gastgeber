#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

struct Room {
    int id;
    char type[20];
};

void reserve();
void display();
void displayAll();
void modify();
void clearRes();
void search();
/* Function to keep the program runing */
char mygetch();

char fname[] = {"vesta.dat"};

void main() {

    int choice;

    while(1) {
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

        scanf("%d", &choice);

        switch(choice) {
            case 1 : reserve();
                break;
            case 0 : exit(0);
                break;
        }
        mygetch();
    }
}

void reserve() {

    struct Room room;

    system("clear");
    printf("*************************************\n");
    printf("*       Hotel Room reservation.     *\n");
    printf("*************************************\n\n");
    printf("Enter Room Number(id):\n");
    scanf("%d", &room.id);
    printf("Enter Room Type: ");
    scanf("%s", &room.type);

    printf("Room %d is type %s", room.id, room.type);
}

char mygetch() {
    char val;
    char rel;

    scanf("%c",&val);
    scanf("%c",&rel);
    return (val);
}
