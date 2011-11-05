#include <stdio.h>
#include <stdlib.h>

#define BUFFER      255

int main() {
    char buffer[BUFFER];
    int number;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        puts("---");
        puts(buffer);
        number = atoi(buffer);
        printf("--- %d\n", number);
    }

    return 0;
}

