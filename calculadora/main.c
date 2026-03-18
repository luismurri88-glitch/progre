#include <stdio.h>
int main()
{
    int a,b,c;
    float f,g,h;
    char op,op2;

    printf("Selecciona una opcion\n");
    printf("1. operaciones con enteros\n");
    printf("2. operaciones con reales\n");
    scanf("%c", &op);

    printf("Selecciona una opcion\n");
    printf("1. suma\n");
    printf("2. resta\n");
    printf("3. multiplicacion\n");
    printf("4. modulo\n");
    printf("5. salir\n");

    scanf("%c",&op2);
    if(op=='1')
    {
        printf("ingresa los dos numeros con los que quieres operar(NUM1,NUM2)");
        scanf("%d %d", &a, &b);
        switch (op2)
        {
        case 1:
            c=a+b;
            break;
        case 2:
            c=a-b;
            break;
        case 3:
            c=a*b;
            break;
        case 4:
            c=a/b;
            break;
        case 5:
            c=a%b;
            break;
        default:
            return 0;
            break;
        }
        printf("el resultado es %d\n", c);
    }else{
        printf("ingresa los dos numeros con los que quieres operar(NUM1,NUM2)");
        scanf("%f %f", &f, &g);
        switch (op2)
        {
        case 1:
            h=f+g;
            break;
        case 2:
            h=f-g;
            break;
        case 3:
            h=f*g;
            break;
        case 4:
            h=f/g;
            break;
        case 5:
            h=(int)f%(int)g;
            break;
        default:
            return 0;
            break;
        }
        printf("el resultado es %f\n", h);
    }
}
