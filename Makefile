# Compile all c files to gastgeber executable!

CC = gcc
CFLAGS = -Wall
OBJ = gastgeber
RM = rm

all:
	$(CC) $(CFLAGS) gastgeber.c userinput.c display.c reserve.c init_dbs.c comparedates.c delete_reservation.c -o $(OBJ) 
exec:

clean:

