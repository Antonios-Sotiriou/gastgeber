/********************
* Build in libraries
*********************/
#include <stdio.h>
#include <stdlib.h>
/*********************
 * Global constants 
 ********************/
#include "header_files/global_vars.h"
/*********************
 * Color Initialisation
 ********************/
#include "header_files/tercon.h"
/*******************************************************
 * My own libraries, collection of functions and structs
 ******************************************************/
#include "header_files/paths.h"

#include "structures/room.h"
#include "structures/guest.h"
#include "structures/day.h"
#include "structures/reservations.h"

#include "header_files/modify.h"
#include "header_files/userinput.h"
#include "header_files/display.h"
// #include "header_files/comparedates.h"
// #include "header_files/init_dbs.h"
// #include "header_files/reserve.h"
// #include "header_files/delete_reservation.h"

void modifyRoom() {

    struct Room room;
    FILE *fp, *fp1;
    fp = fopen(roomsdb, "rb");
    fp1 = fopen(journal_sec, "wb");

    char c;
    while((c = getc(stdin) != '\n') && c != '\t');

    displayModifyRoomLogo();

    printf("Enter room ID you want to modify: ");
    int room_id = getInteger(48, 4);

    if(room_id < 0 || room_id > TOTAL_ROOMS) {
        printf("\n%sRoom id out of range.%s\nDisplay all rooms with option 3 of main menu to see the IDs.\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    } else if(room_id == 0) {
        printf("Watch out for spaces or letters between the numbers.\n");
    } else {
        while(1) {
            fread(&room, sizeof(room), 1, fp);
            if(feof(fp)) {
                break;
            } else if(room.id == room_id) {
                printf(ANSI_COLOR_GREEN "\nRoom Retrieved.\n" ANSI_COLOR_RESET);
                printf("Room ID: %d\n\n", room.id);

                displayModifyRoomChoices();

                char *room_name;
                char *room_type;
                int choice;
                scanf("%d", &choice);
                getc(stdin);
                
                switch(choice) {
                    case 1 : system("clear");
                        printf("%sGive a new Room Name without spaces: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                        room_name = getString(15);
                        if(room_name != 0) {
                            sprintf(room.name, "%s", room_name);
                            printf("\nRoom Name: %s\n", room.name);
                            printf("Room Type: %s\n", room.type);
                            free(room_name);
                            break;
                        } else {
                            printf("\n");
                            return;
                        }
                    case 2 : system("clear");
                        printf("%sGive a new Room Type without spaces: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                        room_type = getString(15);
                        sprintf(room.type, "%s", room_type);
                        printf("\nRoom Name: %s\n", room.name);
                        printf("Room Type: %s\n", room.type);
                        free(room_type);
                        break;
                    case 3 : system("clear");
                        printf("%sGive a new Room Name without spaces: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                        room_name = getString(15);
                        printf("%sGive a new Room Type without spaces: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                        room_type = getString(15);
                        sprintf(room.name, "%s", room_name);
                        sprintf(room.type, "%s", room_type);
                        printf("\nRoom Name: %s\n", room.name);
                        printf("Room Type: %s\n", room.type);
                        free(room_name);
                        free(room_type);
                        break;
                    case 0 :
                        return;
                    default :
                        return;                   
                }
            }
            fwrite(&room, sizeof(room), 1, fp1);
        }
        fclose(fp);
        fclose(fp1);

        printf("\nShall the modifications be applied: [Y/N]?");
        char y = getc(stdin);
        if(y == 'Y' || y == 'y') {
            fp1 = fopen(journal_sec, "rb");
            fp = fopen(roomsdb, "wb");

            while(1) {
                fread(&room, sizeof(room), 1, fp1);
                if(feof(fp1)) {
                    break;
                } else {
                    fwrite(&room, sizeof(room), 1, fp);
                }
            }
            printf(ANSI_COLOR_GREEN "\nModification applied....!\n" ANSI_COLOR_RESET);
            getc(stdin);
            fclose(fp1);
            fclose(fp);
            remove(journal_sec);
            return;
        } else {
            printf(ANSI_COLOR_RED "\nModification canceled!\n" ANSI_COLOR_RESET);
            getc(stdin);
            remove(journal_sec);
            return;
        }
    }
}

