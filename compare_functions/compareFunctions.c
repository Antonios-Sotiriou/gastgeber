int compareDates(char *from_date, char *to_date);

int compareDates(char *from_date, char *to_date) {

    int from_day;
    int from_month;
    int from_year;

    int to_day;
    int to_month;
    int to_year;

    sscanf(from_date, "%d/%d/%d", &from_day, &from_month, &from_year);
    sscanf(to_date, "%d/%d/%d", &to_day, &to_month, &to_year);

    if(to_year < from_year) {
        return 0;
    } else if(to_year ==  from_year && to_month > from_month) {
        return 1;
    } else if(to_year == from_year && to_month == from_month) {
        if(to_day > from_day) {
            return 1;
        } else if(to_day == from_day) {
            printf("Same from and to days.You must provide a day in the future.You can't check in and check out the same day!\n");
            return 0;
        } else {
            printf("This day is in the past!\n");
            return 0;         
        }
    } else {
        printf("To date is older than from date\n");
        return 0;
    }
}