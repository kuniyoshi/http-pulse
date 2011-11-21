#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER        (1 << 16)
#define BUFFER              (1 << 16)
#define MAX_WORDS           9000
#define SEP_CHAR            "\t"

int
split_string(char *chars, char **words) {
    char *cp = chars;
    int length;

    for (length = 0; length < MAX_WORDS; length++) {
        if ((words[length] = strtok(cp, SEP_CHAR)) == NULL) {
            break;
        }

        cp = NULL;
    }

    return length;
}

int
main() {
    char input[INPUT_BUFFER];
    char *words[MAX_WORDS];
    int count_of_words;

    // Read signals from stdin.
    while (fgets(input, INPUT_BUFFER, stdin) != NULL) {
        count_of_words = split_string(input, words);
        printf("%d inputs got.\n", count_of_words);
    }

    return 0;
}

