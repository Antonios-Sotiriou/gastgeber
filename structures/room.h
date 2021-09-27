#ifndef ROOM_H
#define ROOM_H

struct Room {
    int id;
    char type[20];
    int guest_id;
    bool reserved;
    char from_date[20];
    char to_date[20];
};

#endif /* ROOM_H */

