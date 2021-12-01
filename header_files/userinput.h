#ifndef USERINPUT_H
#define USERINPUT_H

void buffer_clear();
int getnuminput(int max_len, bool spaces);
int getformatedDate(char *room_date);
int getInteger(int str_len, int int_len);
char* getString(int str_len);
char* getSpString(int str_len);

#endif /* USERINPUT_H */

