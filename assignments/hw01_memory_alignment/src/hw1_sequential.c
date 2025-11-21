#include<stdio.h>

int main(void)
{
    int var1 = 30; //declaring
    int var2 = 40; //common
    char var3 = 'a'; //variables
    float var4 = 3.5;
    long var5 = 400000;
    int* var6 = &var1;
    double var7 = 45;
    
    printf("Addresses:\n");
    printf("%u\n",&var1); //printing the
    printf("%u\n",&var2); //memory
    printf("%u\n",&var3); //addresses
    printf("%u\n",&var4); //of each
    printf("%u\n",&var5); //variable
    printf("%u\n",&var6);
    printf("%u\n",&var7);
    
    return 0;
}

