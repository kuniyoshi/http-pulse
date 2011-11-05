#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER      255
#define SEP_CHAR    "\t"
#define MAX_WORDS   255

int split_string(char *chars, char **words) {
    const char *sep_char = SEP_CHAR;
    char *cp;
    int length;

    cp = chars;

    for (length = 0; length < MAX_WORDS; length++) {
        if ((words[length] = strtok(cp, sep_char)) == NULL) {
            break;
        }

        cp = NULL;
    }

    return length;
}

int main() {
    char buffer[BUFFER], *words[MAX_WORDS];
    int length, i;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        length = split_string(buffer, words);

        puts("---");

        for (i = 0; i < length; i++) {
            puts(words[i]);
        }
    }

    return 0;
}

