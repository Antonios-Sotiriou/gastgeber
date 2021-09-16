#include <time.h>

int getnuminput();
int getformatedDate(char *room_date);

int getnuminput() {

    char input[10];
    char cleared_num[10];

    fgets(input, sizeof(input), stdin);

    for(int i = 0, d = 0; i <= strlen(input) - 1; i++) {
        // CHECKING FOR ALPHABET
        if ((input[i] >= 65 && input[i] <= 90) || (input[i] >= 97 && input[i] <= 122)) {
            return 0;
        // CHECKING FOR DIGITS
        } else if (input[i] >= 48 && input[i] <= 57) {
            cleared_num[d] = input[i];
            d++;
        // OTHERWISE SPECIAL CHARACTER
        } else {
            continue;
        }
    }
    return atoi(cleared_num);
}
int getformatedDate(char *room_date) { 

    char input[40];
    int day;
    int month;
    int year;
    time_t current_time;
    char converted_day[20];
    char converted_month[20];
    char converted_year[20];

    time(&current_time);

    struct tm *mytime = localtime(&current_time);
    //printf("%i/%i/%i\n", mytime -> tm_mday, mytime -> tm_mon + 1, mytime -> tm_year + 1900);

    fgets(input, 40, stdin);
    int test = sscanf(input, "%d/%d/%d", &day, &month, &year);

    if(test == 3) {
        if(day < 0 || day > 31) {
            printf("Give  a correct day!\n");
            return 0;
        } else if(day > 0 && day < 10) {
            sprintf(converted_day, "0%d/", day);
        } else if(day > 9 && day <= 31) {
            sprintf(converted_day, "%d/", day);
        }
        if(month < 0 || month > 12) {
            printf("Give a correct month!\n");
            return 0;
        } else if(month > 0 && month < 10) {
            sprintf(converted_month, "0%d/", month);
        } else if(month > 9) {
            sprintf(converted_month, "%d/", month);
        }
        if(year > mytime -> tm_year + 1900) {
            sprintf(converted_year, "%d", year);
        } else if(year == mytime -> tm_year + 1900 && month - (mytime -> tm_mon + 1) > 0) {
            sprintf(converted_year, "%d", year);
        } else if(year == mytime -> tm_year + 1900 && month - (mytime -> tm_mon + 1) == 0) {
            if(day - mytime -> tm_mday >= 0) {
                sprintf(converted_year, "%d", year);
            } else {
                printf("This day is in the past!\n");
                return 0;
            }
        } else {
            printf("Provide a date from today to the future!\n");
            return 0;
        }
    } else {
        printf("The date you entered is in wrong format!\n");
        return 0;
    }
    sprintf(room_date, "%s%s%s", converted_day, converted_month, converted_year);
    return 1;
}

