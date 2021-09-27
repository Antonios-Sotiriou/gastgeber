int checkFromDate(struct Reservation res);
int checkAllDates(struct Reservation res);
int applyReservation(int start, int finish, int res_room_id);

int checkFromDate(struct Reservation res) {

    struct Day day;
    FILE *fp;
    fp = fopen(daysdb, "rb");

    int starting_point;
    // While loop to find the startind day;
    while(1) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else if(strcmp(day.date, res.from_date) == 0) {

            for(int i = 1; i <= sizeof(day.room_id) / sizeof(int); i++) {
                // Check if the room id is already in the days rooms ids.
                if(day.room_id[i] == res.room.id) {
                    printf("Room is already reserved for this date: %s\n", day.date);
                    return 1;
                }
            }
        }
    }
    fclose(fp);

    return 0;
}

int checkAllDates(struct Reservation res) {

    struct Day day;
    FILE *fp, *fp1;
    fp = fopen(daysdb, "rb");

    int starting_point;
    int finishing_point;
    // While loop to find the startind and ending days ids;
    while(1) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else if(strcmp(day.date, res.from_date) == 0) {
            starting_point = day.id;
        } else if(strcmp(day.date, res.to_date) == 0) {
            finishing_point = day.id;
        }
    }
    // Go to the beggining of the file and readall data again
    int num_of_days = 0;
    fseek(fp, 0, SEEK_SET);
    while(1) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else {
            // Get the days from starting day id to end day id
            if(day.id >= starting_point && day.id <= finishing_point) {

                for(int i = 1; i <= sizeof(day.room_id) / sizeof(int); i++) {
                    // Check if the room id is already in the days rooms ids.
                    if(day.room_id[i] == res.room.id) {
                       printf("Room is already reserved for those dates: %s\n", day.date);
                       num_of_days++;
                    }
                }
            }
        }
    }
    fclose(fp);

    if(num_of_days != 0) {
        return 1;
    } else {
        printf("Shall the reservation be applied: [Y/N]? ");
        char c = getc(stdin);
        if(c == 'Y' || c == 'y') {
            applyReservation(starting_point, finishing_point, res.room.id);
            getc(stdin);
            return 0;
        } else {
            getc(stdin);
            return 1;
        }
    }
}

int applyReservation(int start, int finish, int res_room_id) {

    struct Day day;
    FILE *fp, *fp1;
    fp = fopen(daysdb, "rb");
    fp1 = fopen("/home/as/c_programs/dbs/journal.dat", "wb");

    while(1) {
        fread(&day, sizeof(day), 1, fp);
        if(feof(fp)) {
            break;
        } else {
            // Get the days from starting day id to end day id
            if(day.id >= start && day.id <= finish) {
                day.room_id[res_room_id] = res_room_id;
            }
            fwrite(&day, sizeof(day), 1, fp1);
        }
    }
    fclose(fp);
    fclose(fp1);

    fp = fopen(daysdb, "wb");
    fp1 = fopen("/home/as/c_programs/dbs/journal.dat", "rb");

    while(1) {
        fread(&day, sizeof(day), 1, fp1);
        if(feof(fp1)) {
            break;
        }
        fwrite(&day, sizeof(day), 1, fp);
    }
    fclose(fp1);
    fclose(fp);

    remove("/home/as/c_programs/dbs/journal.dat");
}
