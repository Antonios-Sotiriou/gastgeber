#ifndef GUEST_H
#define GUEST_H
//#include <stdbool.h>

struct Guest {
    int id;
    char first_name[30];
    char last_name[30];
    char nationality[30];
    bool active;
    bool repeated_guest;
};

#endif /* GUEST_H */

