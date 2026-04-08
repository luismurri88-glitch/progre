#include <stdio.h>
int main (){
float a;
int n=0,p=0;
    do{
scanf("%f", &a);
if (a<0)
{
    n++;
}
else if(a>0){
    p++;
}
    } while (a!=0);
    printf("positivos: %d \n",p);
    printf("negativos: %d \n",n);
    return 0;
}
