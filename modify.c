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

#include "header_files/modify.h"
#include "header_files/userinput.h"
#include "header_files/display.h"

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
                clear_scr();
                printf(ANSI_COLOR_GREEN "\nRoom Retrieved.\n\n" ANSI_COLOR_RESET);
                printf("Room ID: %d\n", room.id);
                printf("Room Name: %s\n", room.name);
                printf("Room Type: %s\n", room.type);
                printf("Room  Capacity: %d\n", room.capacity);
                printf("Room Price: %d\n\n", room.price);

                displayModifyRoomChoices();

                char *room_name;
                char *room_type;
                int choice;
                scanf("%d", &choice);
                getc(stdin);
                
                switch(choice) {
                    case 1 : clear_scr();
                        printf("%sGive a new Room Name without spaces: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                        room_name = getString(20);
                        if(room_name != 0) {
                            sprintf(room.name, "%s", room_name);
                            free(room_name);
                            break;
                        } else {
                            printf("\n");
                            return;
                        }
                    case 2 : clear_scr();
                        printf("%sGive a new Room Type: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                        room_type = getSpString(20);
                        if(room_type != 0) {
                            sprintf(room.type, "%s", room_type);
                            free(room_type);
                            break;
                        } else {
                            printf("\n");
                            return;
                        }
                    case 3 : clear_scr();
                        printf("%sSet Room Capacity: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                        room.capacity = getInteger(48, 2);
                        break;
                    case 4 : clear_scr();
                        printf("%sSet Room Price: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                        room.price = getInteger(48, 10);
                        break;
                    case 5 : clear_scr();
                        printf("%sGive a new Room Name without spaces: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                        room_name = getString(20);
                        if(room_name != 0) {
                            sprintf(room.name, "%s", room_name);
                            free(room_name);
                            printf("%sGive a new Room Type: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                            room_type = getSpString(20);
                            if(room_type != 0) {
                                sprintf(room.type, "%s", room_type);
                                free(room_type);
                                printf("%sSet Room Capacity: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                                room.capacity = getInteger(48, 2);
                                printf("%sSet Room Price: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                                /********************* Must be implement a float function here for prices *****************************/
                                room.price = getInteger(48, 10);
                                break;
                            } else {
                                printf("\n");
                                return;
                            }
                        } else {
                            printf("\n");
                            return;
                        }
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

void modifyGuest() {

    struct Guest guest;
    FILE *fp, *fp1;
    fp = fopen(guestsdb, "rb");
    fp1 = fopen(journal_sec, "wb");

    char c;
    while((c = getc(stdin) != '\n') && c != '\t');

    displayModifyGuestLogo();

    printf("Enter Guest ID you want to modify: ");
    int guest_id = getInteger(48, 4);

    int found = 0;
    while(1) {
        fread(&guest, sizeof(guest), 1, fp);
        if(feof(fp)) {
            break;
        } else if(guest.id == guest_id) {
            printf(ANSI_COLOR_GREEN "\nGuest Retrieved.\n" ANSI_COLOR_RESET);
            printf("Guest ID: %d\n\n", guest.id);
            printf("\nGuest First Name: %s\n", guest.first_name);
            printf("Guest Last Name: %s\n", guest.last_name);
            printf("Guest Last Name: %s\n\n", guest.nationality);
            found = 1;

            displayModifyGuestChoices();

            char *guest_first_name;
            char *guest_last_name;
            char *guest_nationality;

            int choice;
            scanf("%d", &choice);
            getc(stdin);
            
            switch(choice) {
                case 1 : clear_scr();
                    printf("%sSet Guest First Name: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                    /*********************  Must be implement a string function that accepts also spaces here. Guest can have more than 1 name. ******************/
                    guest_first_name = getSpString(20);
                    if(guest_first_name != 0) {
                        sprintf(guest.first_name, "%s", guest_first_name);
                        
                        break;
                    } else {
                        printf("\n");
                        return;
                    }
                case 2 : clear_scr();
                    printf("%sSet Guest Last Name: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                    guest_last_name = getSpString(20);
                    if(guest_last_name != 0) {
                        sprintf(guest.last_name, "%s", guest_last_name);
                        free(guest_last_name);
                        break;
                    } else {
                        printf("\n");
                        return;
                    }
                case 3 : clear_scr();
                    printf("%sSet Guest Nationality: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                    guest_nationality = getSpString(20);
                    if(guest_nationality != 0) {
                        sprintf(guest.nationality, "%s", guest_nationality);
                        free(guest_nationality);
                        break;
                    } else {
                        printf("\n");
                        return;
                    }
                case 4 : clear_scr();
                    printf("%sSet Guest First Name: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                    guest_first_name = getSpString(20);
                    if(guest_first_name != 0) {
                        sprintf(guest.first_name, "%s", guest_first_name);
                        free(guest_first_name);
                        printf("%sSet Guest Last Name: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                        guest_last_name = getSpString(20);
                        if(guest_last_name != 0) {
                            sprintf(guest.last_name, "%s", guest_last_name);
                            free(guest_last_name);
                            printf("%sSet Guest Nationality: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                            guest_nationality = getSpString(20);
                            if(guest_nationality != 0) {
                                sprintf(guest.nationality, "%s", guest_nationality);
                                free(guest_nationality);
                                break;
                            } else {
                                printf("\n");
                                return;
                            }
                        } else {
                            printf("\n");
                            return;
                        }
                    } else {
                        printf("\n");
                        return;
                    }
                case 0 :
                    return;
                default :
                    return;                   
            }
        }
        fwrite(&guest, sizeof(guest), 1, fp1);
    }
    fclose(fp);
    fclose(fp1);

    if(found == 1) {
        printf("\nShall the modifications be applied: [Y/N]?");
        char y = getc(stdin);
        if(y == 'Y' || y == 'y') {
            fp1 = fopen(journal_sec, "rb");
            fp = fopen(guestsdb, "wb");

            while(1) {
                fread(&guest, sizeof(guest), 1, fp1);
                if(feof(fp1)) {
                    break;
                } else {
                    fwrite(&guest, sizeof(guest), 1, fp);
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
    } else {
        printf(ANSI_COLOR_RED "\nNo Guest found with the given ID.\n" ANSI_COLOR_RESET);
    }
}

