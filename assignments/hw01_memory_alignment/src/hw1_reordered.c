#include<stdio.h>

int main(void)
{
    int var1 = 30;
    double var7 = 45;
    long var5 = 400000; //declaring
    int* var6 = &var1; //common
    int var2 = 40; //variables
    float var4 = 3.5;
    char var3 = 'a';
    
    printf("Addresses:\n");
    printf("%u\n",&var1);
    printf("%u\n",&var7); //printing the
    printf("%u\n",&var5); //memory
    printf("%u\n",&var6); //addresses
    printf("%u\n",&var2); //of each
    printf("%u\n",&var4); //variables
    printf("%u\n",&var3);
    
    return 0;
}

