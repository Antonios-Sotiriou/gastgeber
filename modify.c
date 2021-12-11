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
#include <stdbool.h>
/*********************
 * Global constants 
 ********************/
#include "header_files/global/global_vars.h"
#include "header_files/global/rooms_db_path.h"
#include "header_files/global/guests_db_path.h"
/**********************************************
 * Color Initialisation and Terminal management 
 **********************************************/
#include "header_files/tercon.h"
/*******************************************************
 * My own libraries, collection of functions and structs
 ******************************************************/
#include "structures/room.h"
#include "structures/guest.h"
#include "structures/day.h"
#include "structures/reservations.h"

#include "header_files/modify.h"
#include "header_files/userinput.h"
#include "header_files/display.h"
#include "header_files/joins.h"
/* Creates an all rooms struct array and starts the modify room functions algorithm. */
int modifyRoom() {

    struct Room room, rooms[TOTAL_ROOMS];
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, roomsdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate roomsdb file modifyRoom()");
        exit(127);
    }

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

    return result;
}
/* Displays options and manages modification for spesific room of rooms array. */
int modifyRoomPanel(struct Room rooms[]) {

    Terminal term = tercon_init_rows_cols();

    printf(ANSI_MOVE_CURSOR_TO "Enter room ID you want to modify: ", 13, (term.columns - 34) / 2);
    int room_id = getnuminput(6, false);
    int modified = 0;

    if(room_id > TOTAL_ROOMS) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHRoom id out of range.\n"ANSI_COLOR_RESET, term.rows - 2, (term.columns - 20) / 2);
        printf(ANSI_COLOR_RED "\x1b[%d;%dHDisplay all rooms with option 3 of main menu to see the IDs.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 60) / 2);
        return 1;
    } else if (room_id == 0) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHNo room ID provided.Zero ID!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 28) / 2);
        return 1;
    } else if (room_id == -1) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHWrong format.Check for letters.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 31) / 2);
        return 2;
    } else if (room_id == -2) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.Check for special charackters or spaces.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 56) / 2);
        return 3;
    } else if (room_id == -3) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid number length!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
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
                while (modified == 0) {
                    // Get window attributes again here in case user resizes.
                    term = tercon_init_rows_cols();
                    displayModifyRoomChoices(rooms[i]);
                    int choice = getnuminput(4, true);
                    int error = 0;
                    int free_rs = 0;
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
                        tercon_clear_lines(26, 19);
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
                                cap = getnuminput(2, false);
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
                                prc = getnuminput(6, false);
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
                                        cap = getnuminput(2, false);
                                        if (cap != 0) {
                                            /********************* Must be implement a float function here for prices *****************************/
                                            printf(ANSI_COLOR_GREEN "\x1b[%dGSet Room Price in form of (100.00 Currency): " ANSI_COLOR_RESET, (term.columns - 45) / 2);
                                            prc = getnuminput(6, false);
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
        int result = applyRoomModification(rooms);
        return result;
    }
    // return 10 here means Unmodified
    return 10;
}
/* Applies room modification by writing the rooms array to Database. */
int applyRoomModification(struct Room rooms[]) {

    Terminal term = tercon_init_rows_cols();
    printf(ANSI_COLOR_GREEN "\x1b[%d;%dHShall the modifications be applied: [Y/N]?" ANSI_COLOR_RESET, term.rows - 4, (term.columns - 42) / 2);
    char y = getc(stdin);
    if(y == 'Y' || y == 'y') {
        FILE *fp;
        char abs_path[PATH_LENGTH];
        joinHome(abs_path, roomsdb);
        fp = fopen(abs_path, "wb");

        for (int i = 0; i <= TOTAL_ROOMS - 1; i++) {
            fwrite(&rooms[i], sizeof(rooms[i]), 1, fp);
        }
        printf(ANSI_COLOR_GREEN "\x1b[%d;%dHModification applied...!" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 24) / 2);
        fclose(fp);
        return 0;
    } else if (y == '\n' || y == '\t') {
        tercon_clear_error_log();
        printf(ANSI_COLOR_RED "\x1b[%d;%dHModification canceled!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
        // Return 1 here so we can handle in modify() the input buffer accordingly.Otherwise chars are letting in input buffer.
        return 1;
    } else {
        tercon_clear_error_log();
        printf(ANSI_COLOR_RED "\x1b[%d;%dHModification canceled!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
        return 6;
    }
}
/* Creates an all guests Dynamically allocated struct array and starts the modify guest functions algorithm. */
int modifyGuest() {

    struct Guest guest, *guests_par;
    // We creating a dynamically allocated array to store guests because their number is dynamic.
    guests_par = malloc(sizeof(struct Guest));
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, guestsdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate guestsdb file modifyGuest()");
        exit(127);
    }

    int index = 0, dynamic_inc = 2;
    while(1) {
        fread(&guest, sizeof(guest), 1, fp);
        if(feof(fp)) {
            break;
        } else if (guest.id != 0) {
            guests_par = realloc(guests_par, sizeof(struct Guest) * dynamic_inc);
            guests_par[index] = guest;
            index++;
            dynamic_inc++;
        }
    }
    fclose(fp);
    // To make our life easier, here we are copying the dynamically allocated struct array, to a normal struct array,
    // so we can free the resources here, a) to avoid making next functions more complicated, b) avoid creating a ton of else clauses
    // to the following functions and c) also keep truck and make it easier to debugg.
    struct Guest guests[index - 1];
    for(int i = 0; i <= index - 1; i++) {
        guests[i] = guests_par[i];
    }
    free(guests_par);

    displayModifyGuestLogo();

    int result = modifyGuestPanel(guests, index - 1);

    return result;
}
/* Displays options and manages modification for spesific guest of guests array. */
int modifyGuestPanel(struct Guest guests[], int arr_len) {

    Terminal term = tercon_init_rows_cols();
    printf(ANSI_MOVE_CURSOR_TO "Enter Guest ID you want to modify: ", 13, (term.columns - 40) / 2);
    int guest_id = getnuminput(6, false);

    if (guest_id == 0) {
        printf(ANSI_COLOR_RED ANSI_MOVE_CURSOR_TO "No Guest with 0 ID exists!" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 26) / 2);
        return 1;
    } else if (guest_id == -1) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHWrong format.Check for letters.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 31) / 2);
        return 2;
    } else if (guest_id == -2) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.Check for special charackters or spaces.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 56) / 2);
        return 3;
    } else if (guest_id == -3) {
        printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid number length!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
        return 4;
    }

    int found = 0;
    int modified = 0;
    for (int i = 0; i <= arr_len; i++) {
        if(guests[i].id == guest_id) {
            found = 1;

            char *guest_first_name;
            char *guest_last_name;
            char *guest_nationality;
            while(!modified) {
                // Get window attributes again here in case user resizes.
                term = tercon_init_rows_cols();
                // Display guest info here.
                displayModifyGuestChoices(guests[i]);

                int choice = getnuminput(4, true);
                int error = 0;
                int free_rs = 0;
                if (choice == -1) {
                    printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
                    error = 1;
                } else if (choice == -2) {
                    printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
                    error = 2;
                } else if (choice == -3) {
                    printf(ANSI_COLOR_RED "\x1b[%d;%dHInvalid input.getnuminput() error code: %d\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 40) / 2, choice);
                    error = 3;
                } else if ((choice > 4 && choice < 20) || choice > 20) {
                    printf(ANSI_COLOR_RED "\x1b[%d;%dHThis number doesn't corresponds to a choice!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 44) / 2);
                    error = 4;
                } else {
                    tercon_clear_lines(26, 19);
                    switch(choice) {
                        case 1 :
                            printf(ANSI_COLOR_GREEN "\x1b[%dGSet Guest First Name: " ANSI_COLOR_RESET, (term.columns - 22) / 2);
                            guest_first_name = getSpString(20);
                            if(guest_first_name != 0) {
                                sprintf(guests[i].first_name, "%s", guest_first_name);
                                modified = 1;
                            } else {
                                error = 4;
                            }
                            free_rs = 1;
                            break;
                        case 2 :
                            printf(ANSI_COLOR_GREEN "\x1b[%dGSet Guest Last Name: " ANSI_COLOR_RESET, (term.columns - 21) / 2);
                            guest_last_name = getSpString(20);
                            if(guest_last_name != 0) {
                                sprintf(guests[i].last_name, "%s", guest_last_name);
                                modified = 1;
                            } else {
                                error = 4;
                            }
                            free_rs = 2;
                            break;
                        case 3 :
                            printf(ANSI_COLOR_GREEN "\x1b[%dGSet Guest Nationality: " ANSI_COLOR_RESET, (term.columns - 23) / 2);
                            guest_nationality = getSpString(20);
                            if(guest_nationality != 0) {
                                sprintf(guests[i].nationality, "%s", guest_nationality);
                                modified = 1;
                            } else {
                                error = 4;
                            }
                            free_rs = 3;
                            break;
                        case 4 :
                            printf(ANSI_COLOR_GREEN "\x1b[%dGSet Guest First Name: " ANSI_COLOR_RESET, (term.columns - 22) / 2);
                            guest_first_name = getSpString(20);
                            if(guest_first_name != 0) {              
                                printf(ANSI_COLOR_GREEN "\x1b[%dGSet Guest Last Name: " ANSI_COLOR_RESET, (term.columns - 21) / 2);
                                guest_last_name = getSpString(20);
                                if(guest_last_name != 0) {                
                                    printf(ANSI_COLOR_GREEN "\x1b[%dGSet Guest Nationality: " ANSI_COLOR_RESET, (term.columns - 23) / 2);
                                    guest_nationality = getSpString(20);
                                    if(guest_nationality != 0) {
                                        sprintf(guests[i].first_name, "%s", guest_first_name);
                                        sprintf(guests[i].nationality, "%s", guest_nationality);
                                        sprintf(guests[i].last_name, "%s", guest_last_name);
                                        free_rs = 5;
                                        modified = 1;
                                        break;
                                    } else {
                                        free_rs = 5;
                                        error = 4;
                                        break;
                                    }
                                } else {
                                    free_rs = 4;
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
                if (free_rs != 0) {
                    if (free_rs == 1) {
                        free(guest_first_name);
                    } else if (free_rs == 2) {
                        free(guest_last_name);
                    } else if (free_rs == 3) {
                        free(guest_nationality);
                    } else if (free_rs == 4) {
                        free(guest_first_name);
                        free(guest_last_name);
                    } else if (free_rs == 5) {
                        free(guest_first_name);
                        free(guest_last_name);
                        free(guest_nationality);
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
    if(found == 1) {
        int result = applyGuestModification(guests, arr_len);
        return result;
    } else {
        printf(ANSI_COLOR_RED ANSI_MOVE_CURSOR_TO "No Guest found with the given ID.\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 33) / 2);
        // Return 1 here so we can handle in modify() the input buffer accordingly.Otherwise chars are letting in input buffer.
        return 1;
    }
}
/* Applies guest modification by writing the guests array to Database. */
int applyGuestModification (struct Guest guests[], int arr_len) {

    Terminal term = tercon_init_rows_cols();
    printf(ANSI_COLOR_GREEN "\x1b[%d;%dHShall the modifications be applied: [Y/N]?" ANSI_COLOR_RESET, term.rows - 4, (term.columns - 42) / 2);
    char y = getc(stdin);
    if(y == 'Y' || y == 'y') {
        FILE *fp;
        char abs_path[PATH_LENGTH];
        joinHome(abs_path, guestsdb);
        fp = fopen(abs_path, "wb");

        for (int i = 0; i <= arr_len; i++) {
            fwrite(&guests[i], sizeof(guests[i]), 1, fp);
        }
        printf(ANSI_COLOR_GREEN "\x1b[%d;%dHModification applied...!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 24) / 2);
        fclose(fp);
        return 0;
    } else if (y == '\n' || y == '\t') {
        tercon_clear_error_log();
        printf(ANSI_COLOR_RED "\x1b[%d;%dHModification canceled!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
        // Return 1 here so we can handle in modify() the input buffer accordingly.Otherwise chars are letting in input buffer.
        return 1;
    } else {
        tercon_clear_error_log();
        printf(ANSI_COLOR_RED "\x1b[%d;%dHModification canceled!\n" ANSI_COLOR_RESET, term.rows - 1, (term.columns - 22) / 2);
        return 6;
    }
}

