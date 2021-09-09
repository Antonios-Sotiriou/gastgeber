// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

int getnuminput();

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
