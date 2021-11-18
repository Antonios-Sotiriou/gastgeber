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

int modifyRoom() {

    struct Room room, rooms[TOTAL_ROOMS];
    FILE *fp;
    fp = fopen(roomsdb, "rb");

    int index = 0;
    while(1) {
        fread(&room, sizeof(room), 1, fp);
        if (feof(fp)) {
            break;
        } else {
            rooms[index] = room;
            index++;
        }
    }
    fclose(fp);

    displayModifyRoomLogo();
    
    int result = handleRoom(rooms);
    if (result == 20) {
        return 20;
    } else if (result != 0) {
        return result;
    }
    return 0;
}

int handleRoom(struct Room rooms[]) {

    Terminal term = tercon_init_rows_cols();

    printf(ANSI_MOVE_CURSOR_TO "Enter room ID you want to modify: ", 14, (term.columns - 34) / 2);
    int room_id = getInteger(48, 4);
    int modified = 0;

    if(room_id > TOTAL_ROOMS) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHRoom id out of range.\n"ANSI_COLOR_RESET, term.rows - 2, (term.columns - 20) / 2);
        printf(ANSI_COLOR_RED "\x1b[%d;%dHDisplay all rooms with option 3 of main menu to see the IDs.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 60) / 2);
        return 1;
    } else if (room_id == -1) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid number length!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
        return 2;
    } else if (room_id == -2) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHPlease check the syntax!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 24) / 2);
        return 3;
    } else if (room_id == 0) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHNo Room ID provided!\n"ANSI_COLOR_RESET, term.rows - 1, (term.columns - 20) / 2);
        return 4;
    } else {
        for (int i = 0; i <= TOTAL_ROOMS - 1; i++) {
            if (rooms[i].id == room_id) {
                displayRoomInfo(rooms[i]);
                char *room_name;
                char *room_type;
                while (modified == 0) {      
                    displayModifyRoomChoices();              
                    int choice = getnuminput(2);
                    if (choice == -1) {
                        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid number length!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
                    } else if (choice == -2) {
                        printf(ANSI_COLOR_RED "\x1b[%d;%dHPlease check the syntax!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 24) / 2);
                    } else if (choice == -3) {
                        printf(ANSI_COLOR_RED "\x1b[%d;%dHNo Option provided!\n"ANSI_COLOR_RESET, term.rows - 1, (term.columns - 19) / 2);
                    } else if ((choice > 5 && choice < 20) || choice > 20) {
                        printf(ANSI_COLOR_RED "\x1b[%d;%dHThis number doesn't corresponds to a choice!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 44) / 2);
                    } else {     
                        switch(choice) {
                            case 1 :
                                tercon_clear_lines(26, 20);
                                printf(ANSI_COLOR_GREEN "\x1b[%dGGive a new Room Name without spaces: " ANSI_COLOR_RESET, (term.columns - 37) / 2);
                                room_name = getString(20);
                                if (room_name != 0) {
                                    sprintf(rooms[i].name, "%s", room_name);
                                    modified = 1;
                                }
                                free(room_name);
                                break;
                            case 2 :
                                tercon_clear_lines(26, 20);
                                printf(ANSI_COLOR_GREEN "\x1b[%dGGive a new Room Type: " ANSI_COLOR_RESET, (term.columns - 22) / 2);
                                room_type = getSpString(20);
                                if(room_type != 0) {
                                    sprintf(rooms[i].type, "%s", room_type);
                                    modified = 1;
                                }
                                free(room_type);
                                break;
                            case 3 :
                                tercon_clear_lines(26, 20);
                                printf(ANSI_COLOR_GREEN "\x1b[%dGSet Room Capacity: " ANSI_COLOR_RESET, (term.columns - 19) / 2);
                                rooms[i].capacity = getInteger(48, 2);
                                modified = 1;
                            case 4 :
                                tercon_clear_lines(26, 20);
                                printf(ANSI_COLOR_GREEN "\x1b[%dGSet Room Price in form of (100.00 Currency): " ANSI_COLOR_RESET, (term.columns - 45) / 2);
                                rooms[i].price = getInteger(48, 10);
                                modified = 1;
                            case 5 :
                                tercon_clear_lines(26, 20);
                                printf(ANSI_COLOR_GREEN "\x1b[%dGGive a new Room Name without spaces: " ANSI_COLOR_RESET, (term.columns - 37) / 2);
                                room_name = getString(20);
                                if(room_name != 0) {
                                    sprintf(rooms[i].name, "%s", room_name);
                                    free(room_name);
                                    printf(ANSI_COLOR_GREEN "\x1b[%dGGive a new Room Type: " ANSI_COLOR_RESET, (term.columns - 22) / 2);
                                    room_type = getSpString(20);
                                    if(room_type != 0) {
                                        sprintf(rooms[i].type, "%s", room_type);
                                        free(room_type);
                                        printf(ANSI_COLOR_GREEN "\x1b[%dGSet Room Capacity: " ANSI_COLOR_RESET, (term.columns - 19) / 2);
                                        rooms[i].capacity = getInteger(48, 2);
                                        /********************* Must be implement a float function here for prices *****************************/
                                        printf(ANSI_COLOR_GREEN "\x1b[%dGSet Room Price in form of (100.00 Currency): " ANSI_COLOR_RESET, (term.columns - 45) / 2);
                                        rooms[i].price = getInteger(48, 10);
                                        modified = 1;
                                    }
                                }
                            case 20 :
                                return 20;
                            default :
                                break;                  
                        }
                    }
                }
            }
        }
    }
    if (modified) {
        printf(ANSI_COLOR_GREEN "\x1b[%d;%dHShall the modifications be applied: [Y/N]?" ANSI_COLOR_RESET, term.rows - 4, (term.columns - 42) / 2);
        char y = getc(stdin);
        if(y == 'Y' || y == 'y') {
            FILE *fp;
            fp = fopen(roomsdb, "wb");
            for (int i = 0; i <= TOTAL_ROOMS - 1; i++) {
                fwrite(&rooms[i], sizeof(rooms[i]), 1, fp);
            }
            printf(ANSI_COLOR_GREEN "\x1b[%d;%dHModification applied...!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 24) / 2);
            getc(stdin);
            fclose(fp);
            return 0;
        } else if (y == '\n' || y == '\t') {
            tercon_clear_error_log();
            printf(ANSI_COLOR_RED "\x1b[%d;%dHModification canceled!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
            return 9;
        } else {
            tercon_clear_error_log();
            printf(ANSI_COLOR_RED "\x1b[%d;%dHModification canceled!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
            getc(stdin);
            return 9;
        }
    }
    // return 10 here means Unmodified
    return 10;
}

void modifyGuest() {

    struct Guest guest;
    FILE *fp, *fp1;
    fp = fopen(guestsdb, "rb");
    fp1 = fopen(journal_sec, "wb");

    // char c;
    // while((c = getc(stdin) != '\n') && c != '\t');

    displayModifyGuestLogo();

    printf("Enter Guest ID you want to modify: ");
    int guest_id = getInteger(48, 4);

    if (guest_id == 0) {
        printf("No Guest with 0 ID exists!");
        getc(stdin);
        return;
    }

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
                    guest_first_name = getSpString(20);
                    if(guest_first_name != 0) {
                        sprintf(guest.first_name, "%s", guest_first_name);
                        free(guest_first_name);
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

