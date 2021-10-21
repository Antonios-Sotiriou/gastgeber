#ifndef DAY_H
#define DAY_H

struct Day {
    int id;
    char date[20];
    char week_day[6];
    char day[4];
    char month_num[6];
    char month_name[11];
    char year[6];
    int room_id[TOTAL_ROOMS + 1];
};

#endif /* DAY_H */

