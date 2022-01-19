#ifndef MODIFY_H
#define MODIFY_H

int modifyRoom();
int modifyRoomPanel(struct Room rooms[]);
int applyRoomModification(struct Room rooms[]);
int modifyGuest();
int modifyGuestPanel(struct Guest guests[], int arr_len);
int applyGuestModification(struct Guest guests[], int arr_len);

#endif /* MODIFY_H */

