/********************
* Build in libraries
*********************/
#ifdef _WIN32
    #define comparestr stricmp
#else
    #define comparestr strcasecmp
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/*********************
 * Global constants 
 ********************/
#include "header_files/global/global_vars.h"
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
#include "structures/reservations.h"

#include "header_files/init_dbs.h"
#include "header_files/userinput.h"
#include "header_files/handle_guest.h"
#include "header_files/joins.h"
/* Gets Guest informations and checks if their syntax is correct or missing. */
struct Guest handleGuest() {

    struct Guest guest;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, guestsdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate guestsdb file handleGuest()");
        exit(127);
    }

    int next_id = getNextGuestEntry();

    Terminal term = tercon_init_rows_cols();
    // Move cursor to the right position
    tercon_move_y_x(14, (term.columns - 51 ) / 2);
    printf("  Guest First Name: ");
    char *first_name = getSpString(20);
    if (first_name == NULL) {
        tercon_move_y_x(term.rows - 2, (term.columns - 28) / 2);
        printf(ANSI_COLOR_RED "Guest must have a First Name.\n" ANSI_COLOR_RESET);
        guest.active = false;
        free(first_name);
        fclose(fp);
        return guest;
    }
    tercon_move_y_x(15, (term.columns - 51 ) /2);
    printf("  Guest Last Name: ");
    char *last_name = getSpString(20);
    if (last_name == NULL) {
        tercon_move_y_x(term.rows - 2, (term.columns - 29 ) / 2);
        printf(ANSI_COLOR_RED "Guest must have a Last Name.\n" ANSI_COLOR_RESET);
        guest.active = false;
        free(last_name);
        fclose(fp);
        return guest;
    }

    int found = 0;
    while(found == 0) {
        fread(&guest, sizeof(guest), 1, fp);
        if(feof(fp)) {
            break;
        } else if(comparestr(guest.first_name, first_name) == 0 && comparestr(guest.last_name, last_name) == 0) { // Guests can have the same name.Must be implement another check mechanism! 
            tercon_move_y_x(term.rows - 2, (term.columns - 33 ) /2);
            printf(ANSI_COLOR_GREEN "Guest already exists in database." ANSI_COLOR_RESET);
            tercon_move_y_x(term.rows - 1, (term.columns - 31 ) /2);
            printf(ANSI_COLOR_GREEN "Guest marked as Repeated Guest!\n" ANSI_COLOR_RESET);
            guest.repeated_guest = true;
            found = 1;
        }
    }
    fclose(fp);

    if(found == 0) {
        tercon_move_y_x(16, (term.columns - 51 ) /2);
        printf("  Guest Nationality: ");
        char *nationality = getSpString(20);
        if (nationality == NULL) {
            tercon_move_y_x(term.rows - 2, (term.columns - 29 ) / 2);
            printf(ANSI_COLOR_RED "Guest must have a Nationality.\n" ANSI_COLOR_RESET);
            guest.active = false;
            free(first_name);
            free(last_name);
            free(nationality);
            return guest;
        } else {
            sprintf(guest.first_name, "%s", first_name);
            sprintf(guest.last_name, "%s", last_name);
            sprintf(guest.nationality, "%s", nationality);
            guest.active = true;
            free(nationality);
            guest.id = next_id;
        }
    }
    free(first_name);
    free(last_name);
    
    return guest;
}
/* Creates a new guest entry in Database for Reservation res.
    Guest must not be already registered.If he is pass. */
void createGuestEntry(struct Reservation res) {

    struct Guest guest;
    FILE *fp;
    char abs_path[PATH_LENGTH];
    joinHome(abs_path, guestsdb);
    fp = fopen(abs_path, "rb");
    if (fp == NULL) {
        perror("Could not locate guestsdb file createGuestEntry()");
        exit(127);
    }

    int found = 0;
    while(found == 0) {
        fread(&guest, sizeof(guest), 1, fp);
        if(feof(fp)) {
            break;
        } else if(guest.id == res.guest.id) {
            found = 1;
            fclose(fp);
        }
    }

    if(found == 0) {
        fp = fopen(abs_path, "ab");
        guest = res.guest;
        fwrite(&guest, sizeof(guest), 1, fp);
        fclose(fp);
    }
}

