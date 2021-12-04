# Hotel Reservations Software. Version 2.0.0
### The compiler used to run this software is "gcc (Debian 10.3.0-9) 10.3.0"
Only standard C Libraries have been used to develop this software.<br><br>
After you download the software you have to compile the main file to be able to run it.<br>
Please keep in mind that paths maybe vary.This will work from inside the main folder of the<br>
project were the Makefile is present.
To do so run the following command:
```
make all
```
If compiling will be success run the executable with the  **init**  argument to create the databases.<br>
Before you run this command please delete all files from data folder so new databases can be created for you.
```
./gastgeber init
```
If you don't see any errors software is ready for use.
```
./gastgeber
```
From gastgeber.c file you can choose the total number of rooms and also<br>
the years range.As starting year it is advised to use the current year because<br>
noone can make a reservation to the Past unfortunatly.At least so far.

# Enjoy...

