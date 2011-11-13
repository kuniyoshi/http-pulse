#include <stdio.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#define ROWS (1 << 16)
#define COLS (1 << 4)

int main() {
    int csv[ROWS][COLS];
    int i, j;

    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            csv[i][j] = i;
        }
    }

    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            printf("%d\t", csv[i][j]);
        }
        puts("");
    }

    return 0;
}

