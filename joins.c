/********************
* Build in libraries
*********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*********************
 * Global constants 
 ********************/
/**********************************************
 * Color Initialisation and Terminal management 
 **********************************************/
/*******************************************************
 * My own libraries, collection of functions and structs
 ******************************************************/

/* Joins 2 strings together.Resources must be freed after function use. */
char *join(char base[], char tail[]) {

    char *new_path = malloc((strlen(base) - 1) + strlen(tail));

    int i = 0, d = 0, j = 0;

    for (i = 0; i <= strlen(base); i++) {
        if (base[i] != '\0') {
            new_path[j] = base[i];
            j++;
        }
    }
    for (d = 0; d <= strlen(tail); d++) {
        if (tail[d] != '\0') {
            new_path[j] = tail[d];
            j++;
        }
    }
    new_path[j] = '\0';

    return new_path;
}

/* Joins tail to HOME path creating and returning an absolut new_path. */
void joinHome(char new_path[], const char tail[]) {

    char *base = getenv("HOME");

    int i = 0, d = 0, j = 0;

    for (i = 0; i <= strlen(base); i++) {
        if (base[i] != '\0') {
            new_path[j] = base[i];
            j++;
        }
    }
    for (d = 0; d <= strlen(tail); d++) {
        if (tail[d] != '\0') {
            new_path[j] = tail[d];
            j++;
        }
    }
    new_path[j] = '\0';
}

