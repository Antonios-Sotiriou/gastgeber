#ifndef INIT_DBS_H
#define INIT_DBS_H

int get_first_day(int year);
void createDaysDb();
void createRoomsDb();
void createReservationsDb();
void createGuestsDb();
int getNextReservationEntry();
int getNextGuestEntry();

#endif /* INIT_DBS_H */

