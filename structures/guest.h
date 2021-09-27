#ifndef GUEST_H
#define GUEST_H

struct Guest {
    int id;
    char first_name[30];
    char last_name[30];
    char nationality[30];
    bool stammgast;
};

#endif /* GUEST_H */

