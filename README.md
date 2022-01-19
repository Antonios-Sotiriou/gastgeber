# Hotel Reservations Software. Version 2.0.0
### The compiler used to run this software is "gcc (Debian 10.3.0-9) 10.3.0"
Only standard C Libraries have been used to develop this software.<br><br>
As a first step, create a folder named gastgeber to clone the software in.<br>
Thats mantatory for the paths to work properly, without you having to change the global path variables.<br><br>

After you download the software you have to compile the main file to be able to run it.<br>
Please keep in mind that paths maybe vary.This will work from inside the main folder of the<br>
project where the Makefile is present, in our case the gastgeber folder created above.
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
From "header_files/global/global_vars.h" file you can choose the total number of rooms and also<br>
the years range.As starting year it is advised to use the current year.<br>
If you want to change the main folder name, you must change the paths which starting from it.<br>
Paths can be found in "header_files/global/" directory.All files have the word path at end of their names.<br>
There you just replace to each path file the starting name **gastgeber** with anythink you want.<br>

# Enjoy...

