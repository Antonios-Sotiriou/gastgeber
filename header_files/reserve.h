#ifndef RESERVE_H
#define RESERVE_H

int checkFromDate(struct Reservation res);
int checkAllDates(struct Reservation res);
void applyReservation(int start, int finish, int res_room_id);

#endif /* RESERVE_H */

