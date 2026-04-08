#include <stdio.h>
int main (){
    float a=1,i=0;
    while(a!=0){
        scanf("%f",&a);
        i=a+i;
        printf("la suma es: %f",i);
    }
    return 0;
}