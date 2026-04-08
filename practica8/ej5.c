#include <stdio.h>
#include <string.h>

int main (){
char contra[]="4321";
char input[15];
do{
scanf("%15s",input);
} while(strcmp(contra,input)!=0);
printf("contraseña correcta");
return 0;
}