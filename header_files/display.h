#ifndef DISPLAY_H
#define DISPLAY_H

/* Display functions */
void appLogo();
char *displayMainLogo();
Terminal displayRoomReservationLogo();
void displayPreviousStep(struct Reservation res);
void displayRoomInfoLogo();
void displayRoomInfo(struct Room room);
void displayAllRoomsLogo();
void displayAllRoomsInfo(struct Room room);
void displayGuestInfoLogo();
void displayGuestInfo(struct Guest guest);
void displayAllGuestsLogo();
void displayAllGuestsInfo(struct Guest guest);
void displayModifyLogo();
void displayModifyRoomLogo();
void displayModifyRoomChoices(struct Room room);
void displayModifyGuestLogo();
void displayModifyGuestChoices(struct Guest guest);
void displayAllReservationsLogo();
void displayAllReservationsInfo(struct Reservation reservation);
void displayDeleteResLogo();
void displayReservationInfo(struct Reservation reservation);
void displayAnnuallyAvailabillityLogo();
void displayRoomsPerDay(struct Day st_arr[], int i);
void displayRoomAnnuallyReservationsLogo();
void displayRoomAnnuallyReservationsInfo(int room_id, int input_year);
void displayAllRoomsAnnuallyReservationsLogo();
void displayAllRoomsAnnuallyReservationsInfo(int input_year);
/* Global Display functions */
void displayInt(int id, int dis_len);
void displayStr(char *str, int dis_len);
void displayErrorLog();

#endif /* DISPLAY_H */

