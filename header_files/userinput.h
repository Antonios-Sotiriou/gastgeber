#ifndef USERINPUT_H
#define USERINPUT_H

void buffer_clear();
int getnuminput(int max_len, bool spaces);
int getDate(int max_len, char *input);
float getfloat(int max_len);
int getformatedDate(char *room_date);
int getTimelessDate(char *room_date);
char* getString(int str_len);
char* getSpString(int str_len);

#endif /* USERINPUT_H */

