#ifndef DISPLAY_H
#define DISPLAY_H

/* Display functions */
void displayMainLogo();
void displayRoomReservationLogo();
void displayRoomInfoLogo();
void displayRoomInfo(struct Room room);
void displayAllRoomsLogo();
void displayAllRoomsInfo(struct Room room);
void displayModifyLogo();
void displayModifyRoomLogo();
void displayModifyRoomChoices();
void displayAllReservationsLogo();
void displayAllReservationsInfo(struct Reservation reservation);
void displayDeleteResLogo();
void displayAnnuallyAvailabillityLogo();
void displayRoomsPerDay(struct Day st_arr[], int i);
void displayRoomAnnuallyReservationsLogo();
void displayRoomAnnuallyReservationsInfo(int room_id, int input_year);
void displayAllRoomsAnnuallyReservationsLogo();
void displayAllRoomsAnnuallyReservationsInfo(int input_year);
/* Global Display functions */
void displayInt(int id, int dis_len);
void displayStr(char *str, int dis_len);

#endif /* DISPLAY_H */

