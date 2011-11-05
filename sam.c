#include <stdio.h>

#define VOLUME  ((1 << 15) - 1)

int main() {
    int i = VOLUME;
    printf("%d\n", i);

    return 0;
}

