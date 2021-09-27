#ifndef RESERVATIONS_H
#define RESERVATIONS_H

struct Reservation {
    int id;
    struct Guest guest;
    struct Room room;
    char from_date[20];
    char to_date[20];
};

#endif /* RESERVATIONS_H */

