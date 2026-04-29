#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 30
#define HEIGHT 15

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

int snakeX, snakeY; // posición de la primera parte de la vibora
int fruitX, fruitY;
char direction='d';

void increaseScore(){
    static int score=0;
    score++;
        printf("Score %d\n", score);
}

void moveSnake(int *x, int *y, char dir){
    switch(dir)
    {
        case 'w': case 'W': (*y); break;
        
        case 's': case 'S': (*y); break;
            
        case 'a': case 'A': (*x); break;
        
        case 'd': case 'D': (*x); break;
    }
}

void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

#ifdef _WIN32
char readInput(){
    if(_kbhit_()) return _getc();
    return direction;
}
#else
char readInput(){
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON| ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL,0);
    fcntl(STDIN_FILENO, F_GETFL,oldf | O_NONBLOCK);

    ch= getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_GETFL,oldf);
    if(ch!= EOF) return ch;
    return direction;
}
#endif

/**
 * Función que sirve  para iniciar el juego
 */
void setup(){
    snakeX=WIDTH/2;
    snakeY=HEIGHT/2;

    fruitX = (rand()%(WIDTH-2))+1;
    fruitY = (rand()%(HEIGHT-2))+1;
}

void draw(){
    clearScreen();
    for(int i=0; i<WIDTH;i++){
        for(int j=0; j< WIDTH; j++){
            if(j==0 || i ==0 || i==WIDTH-1 || j == HEIGHT-1){
                 printf(YELLOW"#");
                 continue;
            }
            if(snakeX==j && snakeY==i){
                printf(GREEN"O");
                continue;
            } 
            if(fruitX==j && fruitY==i){
                printf(RED"f");
                continue;
            } 
            printf(RESET" ");
        }
        printf(RESET"\n");
    }
    //printf("Presiona W o S o D o A\n");
}



void logic(){

    if(snakeX<=0 || snakeX>=WIDTH || snakeY<=0 || snakeY>=HEIGHT){
        printf("game over\n");
        exit(0);
    }
    if(snakeX == fruitX && snakeY == fruitY){
        fruitX = (rand()%(WIDTH-2))+1;
    fruitY = (rand()%(HEIGHT-2))+1;
        increaseScore();
    }
}



int main(){
    setup();
    while(true){
        draw();
        direction = readInput();
        moveSnake(&snakeX, &snakeY, direction);
        logic();
        #ifdef _WIN32
        Sleep(150);
        #else
        usleep(150000);
        #endif
    }
    return 0;
}