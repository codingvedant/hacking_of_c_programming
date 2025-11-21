#include <string.h>

void jmpesp()
{
    __asm__("jmp *%esp");
}

void copyData(char *arg)
{
    char buffer[80];
    // The vulnerability: no bounds checking
    strcpy(buffer, arg);
}

int main(int argc, char *argv[])
{
    copyData(argv[1]);
    return 0;
}

