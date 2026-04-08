#include <stdio.h>
int main (){
    float a,i=0;
    do{
        scanf("%f",&a);
        i=a+i;
        printf("la suma es: %f",i);
    } while(a!=0);
    return 0;
}