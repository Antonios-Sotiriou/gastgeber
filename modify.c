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
    
    int result = modifyRoomPanel(rooms);
    if (result != 0) {
        return result;
    }
    return 0;
}

int modifyRoomPanel(struct Room rooms[]) {

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
                
                char *room_name;
                char *room_type;
                int cap;
                // Price here in the future should be changed to a float number! 
                int prc;
                // check which resourses must be freed
                int free_rs = 0;
                while (modified == 0) {
                    
                    displayModifyRoomChoices(rooms[i]);
                    int choice = getnuminput(4);
                    int error = 0;
                    if (choice == -1) {
                        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
                        error = 1;
                    } else if (choice == -2) {
                        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
                        error = 2;
                    } else if (choice == -3) {
                        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
                        error = 3;
                    } else if ((choice > 5 && choice < 20) || choice > 20) {
                        printf(ANSI_COLOR_RED "\x1b[%d;%dHThis number doesn't corresponds to a choice!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 44) / 2);
                        error = 4;
                    } else {
                        tercon_clear_lines(26, 20);
                        switch(choice) {
                            case 1 :
                                printf(ANSI_COLOR_GREEN "\x1b[%dGGive a new Room Name without spaces: " ANSI_COLOR_RESET, (term.columns - 37) / 2);
                                room_name = getString(20);
                                if (room_name != 0) {
                                    sprintf(rooms[i].name, "%s", room_name);
                                    modified = 1;
                                } else {
                                    // Error 4 here to use the following error clauses without to consume the buffer because it is already empty.
                                    error = 4;
                                }
                                free_rs = 1;
                                break;
                            case 2 :
                                printf(ANSI_COLOR_GREEN "\x1b[%dGGive a new Room Type: " ANSI_COLOR_RESET, (term.columns - 22) / 2);
                                room_type = getSpString(20);
                                if(room_type != 0) {
                                    sprintf(rooms[i].type, "%s", room_type);
                                    modified = 1;
                                } else {
                                    error = 4;
                                }
                                free_rs = 2;
                                break;
                            case 3 :
                                printf(ANSI_COLOR_GREEN "\x1b[%dGSet Room Capacity: " ANSI_COLOR_RESET, (term.columns - 19) / 2);
                                cap = getInteger(48, 2);
                                if (cap != 0) {
                                    rooms[i].capacity = cap;
                                    modified = 1;
                                } else {
                                    printf(ANSI_COLOR_RED "\x1b[%d;%dHNo Capacity provided or Zero!" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 29) / 2);
                                    error = 4;
                                }
                                break;
                            case 4 :
                                printf(ANSI_COLOR_GREEN "\x1b[%dGSet Room Price in form of (100.00 Currency): " ANSI_COLOR_RESET, (term.columns - 45) / 2);
                                prc = getInteger(48, 10);
                                if (prc != 0) {
                                    rooms[i].price = prc;
                                    modified = 1;
                                } else {
                                    printf(ANSI_COLOR_RED "\x1b[%d;%dHNo Price provided or Zero!" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 26) / 2);
                                    error = 4;
                                }
                                break;
                            case 5 :
                                printf(ANSI_COLOR_GREEN "\x1b[%dGGive a new Room Name without spaces: " ANSI_COLOR_RESET, (term.columns - 37) / 2);
                                room_name = getString(20);
                                if(room_name != 0) {
                                    printf(ANSI_COLOR_GREEN "\x1b[%dGGive a new Room Type: " ANSI_COLOR_RESET, (term.columns - 22) / 2);
                                    room_type = getSpString(20);
                                    if(room_type != 0) {
                                        printf(ANSI_COLOR_GREEN "\x1b[%dGSet Room Capacity: " ANSI_COLOR_RESET, (term.columns - 19) / 2);
                                        cap = getInteger(48, 2);
                                        if (cap != 0) {
                                            /********************* Must be implement a float function here for prices *****************************/
                                            printf(ANSI_COLOR_GREEN "\x1b[%dGSet Room Price in form of (100.00 Currency): " ANSI_COLOR_RESET, (term.columns - 45) / 2);
                                            prc = getInteger(48, 10);
                                            if (prc != 0) {
                                                sprintf(rooms[i].name, "%s", room_name);
                                                sprintf(rooms[i].type, "%s", room_type);
                                                rooms[i].capacity = cap;
                                                rooms[i].price = prc;
                                                free_rs = 3;
                                                modified = 1;
                                                break;
                                            } else {
                                                printf(ANSI_COLOR_RED "\x1b[%d;%dHNo Price provided or Zero!" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 26) / 2);
                                                free_rs = 3;
                                                error = 4;
                                                break;
                                            }
                                        } else {
                                            printf(ANSI_COLOR_RED "\x1b[%d;%dHNo Capacity provided or Zero!" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 29) / 2);
                                            free_rs = 3;
                                            error = 4;
                                            break;
                                        }
                                    } else {
                                        free_rs = 3;
                                        error = 4;
                                        break;
                                    }
                                } else {
                                    free_rs = 1;
                                    error = 4;
                                    break;
                                }
                            case 20 :
                                return 20;
                            default :
                                break;                  
                        }
                    }
                    if (free_rs != 0){
                        if (free_rs == 1) {
                            free(room_name);
                        } else if (free_rs == 2) {
                            free(room_type);
                        } else if (free_rs == 3) {
                            free(room_name);
                            free(room_type);
                        }
                    }
                    if (error != 0) {
                        if (error < 4) {
                            buffer_clear();
                        }
                        tercon_echo_off();
                        printf(ANSI_BLINK_SLOW "\x1b[%d;%dH\x1b[2KPress Enter to continue..." ANSI_BLINK_OFF, term.rows - 4, (term.columns - 26) / 2);
                        buffer_clear();
                        tercon_echo_on();
                    }
                }
            }
        }
    }
    if (modified) {
        // Finally promp user to apply modifications.If he applies this function returns 0;
        applyRoomModification(rooms);
    }
    // return 10 here means Unmodified
    return 10;
}

int applyRoomModification(struct Room rooms[]) {

    Terminal term = tercon_init_rows_cols();
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

