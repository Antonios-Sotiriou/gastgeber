/* Display functions */
void displayMainLogo();
void displayRoomReservationLogo();
void displayRoomInfoLogo();
void displayRoomInfo(struct Room room);
void displayAllRoomsLogo();
void displayAllRoomsInfo(struct Room room);
void displayAllReservationsLogo();
void displayAllReservationsInfo(struct Reservation reservation);
void displayDeleteResLogo();
void displayAnnuallyAvailabillityLogo();
void displayRoomsPerDay(struct Day *st_arr, int i);
/* Global Display functions */
void displayInt(int id, int dis_len);
void displayStr(char *str, int dis_len);

void displayMainLogo() {
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
    printf("5. Display all Reservations\n");
    printf("6. Delete room Reservation\n");
    printf("7. Display Annually Reservations\n");
    printf("8. Search\n");
    printf("0. Exit\n\n");
}
void displayRoomReservationLogo() {
    system("clear");
    printf("*************************************\n");
    printf("*      Room Reservation Panel.      *\n");
    printf("*************************************\n\n");   
}
void displayRoomInfoLogo() {
    system("clear");
    printf("*************************************\n");
    printf("*    Display Room Informations.     *\n");
    printf("*************************************\n\n");
}
void displayRoomInfo(struct Room room) {
    printf(" -----------------------------------------------------------------------------------------------------------------------------\n");
    printf("|      Room ID       |     Room Type      |      Guest ID      |    Room Reserved   |     From Date      |      To Date       |\n");
    printf(" -----------------------------------------------------------------------------------------------------------------------------\n");
    printf("|");
    displayInt(room.id, 20);
    printf("|");
    displayStr(room.type, 20);
    printf("|");
    displayInt(room.guest_id, 20);
    printf("|");
    displayStr(room.reserved ? "true" : "false", 20);
    printf("|");
    displayStr(room.from_date, 20);
    printf("|");
    displayStr(room.to_date, 20);
    printf("|\n");
    printf("------------------------------------------------------------------------------------------------------------------------------\n");
}
void displayAllRoomsLogo() {
    system("clear");
    printf("*************************************\n");
    printf("*  Display All Rooms Informations.  *\n");
    printf("*************************************\n\n");
    printf(" -----------------------------------------------------------------------------------------------------------------------------\n");
    printf("|      Room ID       |     Room Type      |      Guest ID      |    Room Reserved   |     From Date      |      To Date       |\n");
    printf(" -----------------------------------------------------------------------------------------------------------------------------\n");
}
void displayAllRoomsInfo(struct Room room) {
    printf("|");
    displayInt(room.id, 20);
    printf("|");
    displayStr(room.type, 20);
    printf("|");
    displayInt(room.guest_id, 20);
    printf("|");
    displayStr(room.reserved ? "true" : "false", 20);
    printf("|");
    displayStr(room.from_date, 20);
    printf("|");
    displayStr(room.to_date, 20);
    printf("|\n");
    printf("------------------------------------------------------------------------------------------------------------------------------\n");
}
void displayAllReservationsLogo() {
    system("clear");
    printf("*************************************\n");
    printf("*      Display All Reservations.    *\n");
    printf("*************************************\n\n");
    printf(" --------------------------------------------------------------------------------------------------------\n");
    printf("|   Reservation ID   |      Room Id       |      Guest ID      |     From Date      |      To Date       |\n");
    printf(" --------------------------------------------------------------------------------------------------------\n");
}
void displayAllReservationsInfo(struct Reservation reservation) {
    printf("|");
    displayInt(reservation.id, 20);
    printf("|");
    displayInt(reservation.room.id, 20);
    printf("|");
    displayInt(reservation.guest.id, 20);
    printf("|");
    displayStr(reservation.from_date, 20);
    printf("|");
    displayStr(reservation.to_date, 20);
    printf("|\n");
    printf("---------------------------------------------------------------------------------------------------------\n");
}
void displayDeleteResLogo() {
    system("clear");
    printf("*************************************\n");
    printf("*         Delete Reservation.       *\n");
    printf("*************************************\n\n");
}
void displayAnnuallyAvailabillityLogo() {
    system("clear");
    printf("*************************************\n");
    printf("*       Annually Reservations.      *\n");
    printf("*************************************\n\n");   
}
void displayRoomsPerDay(struct Day *st_arr, int i) {
    
    for(int x = 0; x <= i - 1; x++) {
        char month[11];
        int month_days = 0;
        if(strcmp(st_arr[x].day, "01") == 0) {
            printf("\n\n\t\t\t\t\t\t\t\t\t%s\n", st_arr[x].month_name);
            while (strcmp(st_arr[x].month_name, month) != 0) {
                sprintf(month, "%s", st_arr[x].month_name);
                for(int d = 0; d <= i - 1; d++) {
                    if(strcmp(st_arr[d].month_name, month) == 0) {
                        printf("| %s ", st_arr[d].day);
                        month_days++;
                    }
                }
            }
            printf("|\n");
            for(int m = 0; m <= month_days * 5; m++) {
                printf("-");
            }
            printf("\n|");
        }
        int num_of_rooms = 0;
        for(int z = 0; z <= sizeof(st_arr[x].room_id) / sizeof(int) - 1; z++) {
            if(st_arr[x].room_id[z] >= 1 && st_arr[x].room_id[z] <= TOTAL_ROOMS) {
                num_of_rooms++;
            }
        }
        if(num_of_rooms == 0) {
            printf(ANSI_COLOR_RED " %d  " ANSI_COLOR_RESET, num_of_rooms);
            printf("|");
        } else {
            displayInt(num_of_rooms, 4);
            printf("|");
        }
    }
    free(st_arr);  
}
void displayInt(int id, int dis_len) {

    char converted[30];

    sprintf(converted, "%d", id);
    int d = (dis_len - strlen(converted)) / 2;

    for(int i = 0; i <= dis_len - strlen(converted); i++) {
        if(i < d) {
            printf(" ");
        } else if(i > d) {
            printf(" ");
        } else {
            printf(ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET, converted);
        }
    }
}
void displayStr(char *str, int dis_len) {

    int d = (dis_len - strlen(str)) / 2;

    for(int i = 0; i <= dis_len - strlen(str); i++) {
        if(i < d) {
            printf(" ");
        } else if(i > d) {
            printf(" ");
        } else {
            printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, str);
        }
    }
}