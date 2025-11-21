#include <stdio.h>

struct __attribute__((packed)) MyVars
{
    char var3;
    int var2;
    float var4; //declaring a
    long var5; //packed
    int* var6; //structure
    double var7;
    int var1;
};

int main(void)
{
    struct MyVars vars;
    
    printf("Addresses:\n");
    printf("%p\n", &vars.var3); //printing
    printf("%p\n", &vars.var2); //the
    printf("%p\n", &vars.var4); //memory
    printf("%p\n", &vars.var5); //addresses
    printf("%p\n", &vars.var6);
    printf("%p\n", &vars.var7);
    printf("%p\n", &vars.var1);
    
    return 0;
}

