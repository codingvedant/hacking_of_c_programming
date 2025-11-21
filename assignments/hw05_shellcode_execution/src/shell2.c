#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char shellcode[] =
        "\xb8\x01\x00\x00\x00" /* mov eax, 1 */
        "\xbb\x02\x00\x00\x00" /* mov ebx, 2 */
        "\xcd\x80";             /* int 0x80 */
    
    int (*shell)(void) = (void *)&shellcode;
    return shell();
}

