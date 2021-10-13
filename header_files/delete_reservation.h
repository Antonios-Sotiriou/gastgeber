#ifndef DELETE_RESERVATION_H
#define DELETE_RESERVATION_H

void getResDatesToDelete(struct Reservation res);
void deleteRoomFromDates(int start, int finish, int res_room_id);
void applyReservationDelete(int res_id);

#endif /* DELETE_RESERVATION_H */

