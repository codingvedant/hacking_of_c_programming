#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *gets(char *s);

int main(int argc, char **argv) {
    char buffer[32];
    gets(buffer);
    printf("Your data is %d bytes.\n", strlen(buffer));
    puts(buffer);
    return 0;
}

