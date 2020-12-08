#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

// compilar con gcc main.c utils.c

/* hice un struct para aprender a usarlos pero es lo mismo que hacer 3 variables separadas
 * int bomb_x
 * int bomb_y
 * int bomb_time
 */
struct bomb {
    int x;
    int y;
    int time;
};

int main() {
    // se inicializan las variables y se cambia la semilla de rand().
    srand(time(NULL));
    int map_w = 15, map_h = 15;
    char **map, option;
    int turnos = 300;
    int player_x = 1, player_y = 1;
    struct bomb bomb;
    while (1) {
        // se llama al comando externo clear para limpiar la pantalla. no se si funcione en windows.
        system("clear");
        printf("BOMBERMAN\n\n");
        printf("1. Iniciar Juego\n2. Opciones\n3. Salir\n");
        option = getchar();

        // salir del juego.
        if (option == '3') {
            system("clear");
            return 0;

        // menu de opciones.
        } else if (option == '2') {
            while (1) {
                system("clear");
                printf("1. Tamaño del mapa = %dx%d\n2. Turnos = %d\n3. Atras\n", map_w, map_h, turnos);
                option = getchar();

                // volver atras
                if (option == '3')
                    break;
                
                // cambiar turnos del juego
                else if (option == '2') {
                    system("clear");
                    printf("Cantidad de turnos: ");
                    scanf("%d", &turnos);
                }

                // cambiar tamaño del mapa
                else if (option == '1') {
                    system("clear");
                    printf("Tamaño del mapa, w x h : ");
                    scanf("%d %d", &map_w, &map_h);
                }
            }

        // iniciar juego.
        } else if (option == '1') {
            system("clear");
            // se crea el mapa en el heap.
            map = create_map(map_h, map_w); // funcion definida en utils.c
            player_x = 1;
            player_y = 1;
            map[player_x][player_y] = '@';
            bomb.x = 1;
            bomb.y = 1;
            bomb.time = -1;
            // ciclo for. Dura la cantidad de turnos del juego.
            for (int i = 0; i < turnos; i++) {
                system("clear");
                int prev_x = player_x, prev_y = player_y; // se guarda la posicion actual del jugador.

                printf("DEBUG\n");
                printf("Turnos: %d, player_x = %d, player_y = %d, option = %c\n", turnos - i, player_x, player_y, option);
                
                // se imprime el mapa en pantalla.
                print_map(map, map_w, map_h);
                printf("Moverse (wasd)\nBomba (q)\nEsperar (e)\nSalir (b)\n");

                scanf(" "); // esto es para consumir el caracter '\n' que se genera al ingresar un caracter.
                option = getchar();
                if (option == 'w' || option == 'a' || option == 's' || option == 'd') {
                    switch (option) {
                        case 'a': player_y--; break;
                        case 'd': player_y++; break;
                        case 'w': player_x--; break;
                        case 's': player_x++; break;
                        default: break;
                    }
                    
                    // el jugador solo puede moverse a un espacio en blanco.
                    if (map[player_x][player_y] != ' ') {
                        player_x = prev_x;
                        player_y = prev_y;
                    } else {
                        if (map[prev_x][prev_y] != 'Q')
                            map[prev_x][prev_y] = ' ';
                        map[player_x][player_y] = '@';
                    }
                }

                // se planta una bomba solo si no hay otra en pantalla.
                else if (option == 'q' && bomb.time == -1) {
                    bomb.x = player_x;
                    bomb.y = player_y;
                    bomb.time = 4;
                    map[player_x][player_y] = 'Q';
                }

                else if (option == 'b')
                    break;
                
                // se resta 1 al tiempo de la bomba en cada turno.
                if (bomb.time > 0)
                    bomb.time--;
                
                // cuando llega a 0 explota.
                else if (bomb.time == 0) {
                    // no se me ocurrio como hacerlo con un ciclo asi que lo hice a mano.
                    // si la bomba alcanza un bloque blando, se destruye.
                    if (map[bomb.x-1][bomb.y] == '%')
                        map[bomb.x-1][bomb.y] = ' ';
                    if (map[bomb.x+1][bomb.y] == '%')
                        map[bomb.x+1][bomb.y] = ' ';
                    if (map[bomb.x][bomb.y-1] == '%')
                        map[bomb.x][bomb.y-1] = ' ';
                    if (map[bomb.x][bomb.y+1] == '%')
                        map[bomb.x][bomb.y+1] = ' ';

                    // si la bomba alcanza al jugador, termina el juego.
                    if (bomb.x-1 == player_x && bomb.y == player_y)
                        break;
                    if (bomb.x+1 == player_x && bomb.y == player_y)
                        break;
                    if (bomb.x == player_x && bomb.y-1 == player_y)
                        break;
                    if (bomb.x == player_x && bomb.y+1 == player_y)
                        break;
                    if (bomb.x == player_x && bomb.y == player_y)
                        break;
                    
                    // se elimina el caracter 'Q' del mapa y se deja el tiempo de la bomba en -1.
                    map[bomb.x][bomb.y] = ' ';
                    bomb.time--;
                }                
            }

            // al terminar el juego se libera la memoria ocupada por el mapa.
            for (int i = 0; i < map_h; i++)
                free(*(map + i));
            free(map);
        }
    }

    return 0;
}
