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
/*********************
 * Global constants 
 ********************/

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
#include "structures/reservations.h"

#include "header_files/init_dbs.h"
#include "header_files/userinput.h"
#include "header_files/handle_guest.h"

struct Guest handleGuest() {

    struct Guest guest;
    FILE *fp;
    fp = fopen(guestsdb, "rb");

    int next_id = getNextGuestEntry();

    printf("Guest First Name: ");
    char *first_name = getString(15);
    printf("Guest Last Name: ");
    char *last_name = getString(15);

    int found = 0;
    while(found == 0) {
        fread(&guest, sizeof(guest), 1, fp);
        if(feof(fp)) {
            break;
        } else if(comparestr(guest.first_name, first_name) == 0 && comparestr(guest.last_name, last_name) == 0) { // Guests can have the same name.Must be implement another check mechanism! 
            printf("\nGuest already exists in database.\nReservation must be marked as Repeated Guest!\n\n");
            found = 1;
        }
    }
    fclose(fp);

    if(found == 0) {
        printf("Guest Nationality: ");
        char *nationality = getString(15);
        sprintf(guest.first_name, "%s", first_name);
        sprintf(guest.last_name, "%s", last_name);
        sprintf(guest.nationality, "%s", nationality);
        free(nationality);
        guest.id = next_id;
    }
    free(first_name);
    free(last_name);

    return guest;
}

void createGuestEntry(struct Reservation res) {

    struct Guest guest;
    FILE *fp;
    fp = fopen(guestsdb, "rb");

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
        fp = fopen(guestsdb, "ab");
        guest = res.guest;
        fwrite(&guest, sizeof(guest), 1, fp);
        fclose(fp);
    }

}

