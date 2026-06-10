#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILAS 20
#define COLUMNAS 20
#define TAMANIO_CELDA 24

// Matriz global (Sección 5.2)
char mapa[FILAS][COLUMNAS];

// Estado del juego e Indicadores (Secciones 5.6 y 5.7)
int pacman_x = 0, pacman_y = 0;
int fantasma_x = 0, fantasma_y = 0;
int puntaje = 0;
int vidas = 3;
int total_pellets = 0;

// Prototipos obligatorios de funciones del Plan Modular (Sección 7.4)
void cargarMapa();
void moverJugador(int dx, int dy);
void moverFantasma();
int detectarColisiones(int nueva_x, int nueva_y);
void renderizarMapa(SDL_Renderer* renderer);
void guardarPuntaje();

int main(int argc, char* argv[]) {
    // Inicializar semilla aleatoria para el comportamiento de los fantasmas
    srand(time(NULL));

    cargarMapa();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error al iniciar SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* ventana = SDL_CreateWindow(
        "Pac-Man UNAM - Finalizado",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        COLUMNAS * TAMANIO_CELDA, FILAS * TAMANIO_CELDA,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

    int juego_ejecutandose = 1;
    SDL_Event evento;

    printf("\n--- ¡PARTIDA INICIADA! ---\n");
    printf("Vidas iniciales: %d | Pellets en mapa: %d\n\n", vidas, total_pellets);

    // Ciclo Principal (Sección 7.2)
    while (juego_ejecutandose) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                juego_ejecutandose = 0;
            } 
            else if (evento.type == SDL_KEYDOWN) {
                switch (evento.key.keysym.sym) {
                    case SDLK_UP:    moverJugador(0, -1); break;
                    case SDLK_DOWN:  moverJugador(0, 1);  break;
                    case SDLK_LEFT:  moverJugador(-1, 0); break;
                    case SDLK_RIGHT: moverJugador(1, 0);  break;
                    case SDLK_ESCAPE: juego_ejecutandose = 0; break;
                }
            }
        }

        // --- LÓGICA DE FIN DE JUEGO (Sección 5.8) ---
        // 1. Condición de Derrota (Sin vidas)
        if (vidas <= 0) {
            printf("\n=========================\n");
            printf("   GAME OVER - DERROTA   \n");
            printf("=========================\n");
            juego_ejecutandose = 0;
            break;
        }

        // 2. Condición de Victoria (Sin pellets)
        if (total_pellets <= 0) {
            printf("\n=========================\n");
            printf("   ¡VICTORIA EN EL NIVEL!  \n");
            printf("=========================\n");
            juego_ejecutandose = 0;
            break;
        }

        // Renderizado básico continuo
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderizarMapa(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Al terminar, persistencia de datos (Sección 5.9)
    guardarPuntaje();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();

    return 0;
}

void cargarMapa() {
    FILE* archivo = fopen("mapa.txt", "r");
    if (archivo == NULL) {
        printf("Error crítico: Falta archivo mapa.txt\n");
        exit(1);
    }

    total_pellets = 0; // Reiniciar contador por seguridad

    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLUMNAS; c++) {
            char ch = fgetc(archivo);
            if (ch == '\n' || ch == '\r') {
                c--;
                continue;
            }
            mapa[f][c] = ch;

            if (ch == 'P') {
                pacman_x = c;
                pacman_y = f;
                mapa[f][c] = ' '; // Vaciar casilla para movimiento libre
            } 
            else if (ch == 'F') {
                fantasma_x = c;
                fantasma_y = f;
                mapa[f][c] = ' '; // Vaciar casilla para que el fantasma se mueva libremente
            } 
            else if (ch == '.') {
                total_pellets++; // Contabilizar pellets totales para la victoria
            }
        }
    }
    fclose(archivo);
}

int detectarColisiones(int nueva_x, int nueva_y) {
    if (nueva_x < 0 || nueva_x >= COLUMNAS || nueva_y < 0 || nueva_y >= FILAS) {
        return 0; 
    }
    if (mapa[nueva_y][nueva_x] == '#') {
        return 0; 
    }
    return 1;
}

void moverJugador(int dx, int dy) {
    int nueva_x = pacman_x + dx;
    int nueva_y = pacman_y + dy;

    if (detectarColisiones(nueva_x, nueva_y)) {
        pacman_x = nueva_x;
        pacman_y = nueva_y;

        // Consumo de pellets e indicadores
        if (mapa[pacman_y][pacman_x] == '.') {
            mapa[pacman_y][pacman_x] = ' ';
            puntaje += 10;
            total_pellets--;
            printf("[PUNTOS] Pellet devorado. Total: %d | Quedan: %d\n", puntaje, total_pellets);
        }
    }

    // Cada vez que Pac-Man ejecuta un movimiento, los fantasmas responden (Sección 5.5)
    moverFantasma();
}

// Lógica e Inteligencia Artificial básica sugerida (Sección 5.5.2)
void moverFantasma() {
    int dx = 0, dy = 0;
    
    // Comportamiento mixto: 70% Persecución dirigida, 30% Movimiento Errático/Aleatorio
    if ((rand() % 100) < 70) {
        // Algoritmo por reglas simples basado en diferencias de coordenadas (Pseudocódigo del PDF)
        if (pacman_x < fantasma_x) dx = -1;
        else if (pacman_x > fantasma_x) dx = 1;
        else if (pacman_y < fantasma_y) dy = -1;
        else if (pacman_y > fantasma_y) dy = 1;
    } else {
        // 30% del tiempo escoge una dirección completamente al azar
        int dir = rand() % 4;
        if (dir == 0) dy = -1;      // Arriba
        else if (dir == 1) dy = 1;  // Abajo
        else if (dir == 2) dx = -1; // Izquierda
        else if (dir == 3) dx = 1;  // Derecha
    }

    int nueva_fx = fantasma_x + dx;
    int nueva_fy = fantasma_y + dy;

    // Validación de colisión del fantasma contra paredes
    if (detectarColisiones(nueva_fx, nueva_fy)) {
        fantasma_x = nueva_fx;
        fantasma_y = nueva_fy;
    }

    // Verificación de colisión mortal inmediata (Jugador choca con Fantasma)
    if (pacman_x == fantasma_x && pacman_y == fantasma_y) {
        vidas--;
        printf("\n💥 ¡ALERTA! El fantasma te ha atrapado. Vidas restantes: %d\n\n", vidas);
        
        // Reinicio estructural de coordenadas de la partida en caso de pérdida de vida
        pacman_x = 1;
        pacman_y = 1;
        fantasma_x = 18;
        fantasma_y = 17;
    }
}

// Guardado persistente exigido en archivos planos (Sección 5.9)
void guardarPuntaje() {
    FILE* archivo = fopen("puntajes.txt", "a"); // "a" añade récords al final sin borrar previos
    if (archivo == NULL) {
        printf("Error al intentar escribir en puntajes.txt\n");
        return;
    }
    fprintf(archivo, "Jugador_UNAM - Record Final: %d puntos\n", puntaje);
    fclose(archivo);
    printf("Registro guardado con éxito en 'puntajes.txt'.\n");
}

void renderizarMapa(SDL_Renderer* renderer) {
    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLUMNAS; c++) {
            SDL_Rect celda = { c * TAMANIO_CELDA, f * TAMANIO_CELDA, TAMANIO_CELDA, TAMANIO_CELDA };

            if (mapa[f][c] == '#') {
                SDL_SetRenderDrawColor(renderer, 0, 0, 150, 255); // Pared Azul
                SDL_RenderFillRect(renderer, &celda);
            } 
            else if (mapa[f][c] == '.') {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Pellet Blanco
                SDL_Rect punto = { celda.x + 10, celda.y + 10, 4, 4 };
                SDL_RenderFillRect(renderer, &punto);
            }
        }
    }

    // Dibujado dinámico del Fantasma activo (Bloque Rojo)
    SDL_Rect rect_fantasma = { fantasma_x * TAMANIO_CELDA, fantasma_y * TAMANIO_CELDA, TAMANIO_CELDA, TAMANIO_CELDA };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect_fantasma);

    // Dibujado dinámico de Pac-Man activo (Bloque Amarillo)
    SDL_Rect rect_pacman = { pacman_x * TAMANIO_CELDA, pacman_y * TAMANIO_CELDA, TAMANIO_CELDA, TAMANIO_CELDA };
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect_pacman);
}