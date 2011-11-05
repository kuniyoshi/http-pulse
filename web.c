#include <stdio.h>
#include <string.h>

#define MAX_LEN  100

int main() {
    int i, len;
    char str[] = "This is a test", *words[MAX_LEN], *cp;
    const char *delim = " ";
    cp = str;
    for (len = 0; len < MAX_LEN; len++) {
        if ((words[len] = strtok(cp, delim)) == NULL)
            break;
        cp = NULL;
    }
    for (i=0; i<len; i++) {
        puts(words[i]);
    }
    return 0;
}
