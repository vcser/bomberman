#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include "utils.h"
#include "gamelogic.h"
#include "graphics.h"

extern int game, running, options, turns;

static struct map map = {25,25};

static struct player player1;

void run_game() {
    // se inicializan las variables
    
    player1.texture = load_image("assets/player.png"); // textura del jugador
    // posicion inicial del jugador.
    player1.x = 1;
    player1.y = 1;
    player1.speed = 1;

    // se crea el mapa
    map.map = create_map(map.h, map.w);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    map.texture = update_map_texture(&map);
    //SDL_Rect camera = {0,0,17*16,13*16}; //map_w*block_w, map_h*block_h

    struct bomb bomb;
    bomb.x = 1;
    bomb.y = 1;
    bomb.time = -1;
    bomb.texture = load_image("assets/bomb.png");

    player1.bombs = &bomb;

    while (game) {

        SDL_Point prev = {player1.x, player1.y};
        int moved = 0;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT){
                game = 0;
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_DOWN:
                        player1.y++;
                        moved = 1;
                        break;
                    case SDLK_UP:
                        player1.y--;
                        moved = 1;
                        break;
                    case SDLK_RIGHT:
                        player1.x++;
                        moved = 1;
                        break;
                    case SDLK_LEFT:
                        player1.x--;
                        moved = 1;
                        break;
                    case SDLK_x:
                        //turns--;
                        moved = 1;
                        break;
                    case SDLK_z:
                        if (bomb.time == -1) {
                            bomb.x = player1.x;
                            bomb.y = player1.y;
                            bomb.time = 5;
                            map.map[bomb.y][bomb.x] = 'Q';
                            SDL_DestroyTexture(map.texture);
                            map.texture = update_map_texture(&map);
                        }
                        break;
                    case SDLK_ESCAPE:
                        game = 0;
                        break;
                }          
            }
        }

        if (map.map[player1.y][player1.x] != ' ') {
            player1.x = prev.x;
            player1.y = prev.y;
            moved = 0;
        } else if (moved) {
            turns--;
        }
        
        if (bomb.time > 0 && moved) {
            bomb.time--;
            moved = 0;
        } else if (bomb.time == 0) {
            // si la bomba alcanza un bloque blando, se destruye.
            if (map.map[bomb.y][bomb.x-1] == '%')
                map.map[bomb.y][bomb.x-1] = ' ';
            if (map.map[bomb.y][bomb.x+1] == '%')
                map.map[bomb.y][bomb.x+1] = ' ';
            if (map.map[bomb.y-1][bomb.x] == '%')
                map.map[bomb.y-1][bomb.x] = ' ';
            if (map.map[bomb.y+1][bomb.x] == '%')
                map.map[bomb.y+1][bomb.x] = ' ';

            // si la bomba alcanza al jugador, termina el juego.
            if (bomb.x-1 == player1.x && bomb.y == player1.y)
                break;
                //printf("muerto\n");
            else if (bomb.x+1 == player1.x && bomb.y == player1.y)
                break;
                //printf("muerto\n");
            else if (bomb.x == player1.x && bomb.y-1 == player1.y)
                break;
                //printf("muerto\n");
            else if (bomb.x == player1.x && bomb.y+1 == player1.y)
                break;
                //printf("muerto\n");
            else if (bomb.x == player1.x && bomb.y == player1.y)
                break;
                //printf("muerto\n");
            
            // se elimina el caracter 'Q' del mapa y se deja el tiempo de la bomba en -1.
            map.map[bomb.y][bomb.x] = ' ';
            bomb.time--;
            SDL_DestroyTexture(map.texture);
            map.texture = update_map_texture(&map);
        }


        // se renderiza el mapa y el jugador
        SDL_RenderClear(renderer);

        camera_present(&player1, &map);


        //////////////////////////////
        char *str = (char*) malloc(4*sizeof(char));
        snprintf(str, 4, "%d", player1.x);
        print_text(str, (SDL_Rect) {0,0}, 3, (SDL_Color) {255,0,0,255});
        snprintf(str, 4, "%d", player1.y);
        print_text(str, (SDL_Rect) {0,1*16*3}, 3, (SDL_Color) {255,0,0,255});
        snprintf(str, 4, "%d", turns);
        print_text(str, (SDL_Rect) {0,2*16*3}, 3, (SDL_Color) {255,0,0,255});
        snprintf(str, 4, "%d", bomb.time);
        print_text(str, (SDL_Rect) {0,3*16*3}, 3, (SDL_Color) {255,0,0,255});
        free(str);
        //////////////////////////////


        //SDL_RenderSetViewport(renderer, &camera);
        SDL_RenderPresent(renderer);

        if (turns <= 0) {
            game = 0;
            printf("Se acabaron los turnos\n");
        }
    }

    turns = 300;

    //liberar memoria del mapa y terminar el juego.
    for (int i = 0; i < map.h; i++)
            free(*(map.map + i));
    free(map.map);
    SDL_DestroyTexture(map.texture);
    SDL_SetWindowTitle(window, "BOMBERMAN");
    SDL_DestroyTexture(player1.texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    game = 0;
}



char **create_map(int map_h, int map_w) {
    // se reserva la memoria en el heap para un arreglo 2d.
    char **map = (char**) malloc(map_h * sizeof(char*));
    for (int i = 0; i < map_h; i++) {
        *(map + i) = (char*) malloc(map_w * sizeof(char));
    }

    // se inicializa el arreglo con el caracter ' '.
    for (int i = 0; i < map_h; i++)
        for (int j = 0; j < map_w; j++)
            map[i][j] = ' ';
    
    // se llena la primera fila con el caracter '#' (bloque indestructible).
    for (int i = 0; i < map_w; i++)
        map[0][i] = '#';

    // lo mismo para la ultima fila.
    for (int i = 0; i < map_w; i++)
        map[map_h-1][i] = '#';
    

    // se crea el patron de bloques solidos del mapa
    for (int i = 1; i < map_h - 1; i++)
        for (int j = 0; j < map_w; j++) {
            if (i%2 == 1) {
                map[i][0] = '#';
                map[i][map_w-1] = '#';
                break;
            } else
                if (j%2 == 0)
                    map[i][j] = '#';
        }
    

    // se llena al azar con bloques blandos '%'.
    for (int i = 1; i < map_h - 1; i++)
        for (int j = 1; j < map_w - 1; j++)
            // si el espacio esta vacio y rand() es par (50 y 50), se cambia por un bloqe blando.
            if (map[i][j] == ' ' && rand()%2)
                map[i][j] = '%';
    
    // esto es para asegurarse que la esquina donde aparece el jugador este vacia.
    map[1][1] = ' ';
    map[1][2] = ' ';
    map[2][1] = ' ';

    return map;
}

// menu de opciones.
void options_menu() {
    int selection = 3;

    while (options) {
        int inc = 0;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT){
                options = 0;
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_DOWN:
                        if (selection < 3)
                            selection++;
                        break;
                    case SDLK_UP:
                        if (selection > 0)
                            selection--;
                        break;
                    case SDLK_RIGHT:
                        inc = 1;
                        break;
                    case SDLK_LEFT:
                        inc = -1;
                        break;
                    case SDLK_z:
                    case SDLK_RETURN:
                        if (selection == 3)
                            options = 0;
                        break;
                    case SDLK_x:
                    case SDLK_ESCAPE:
                        options = 0;
                        break;
                }
            }
        }

        if (inc) {
            switch (selection) {
                case 0:
                    if (map.w > 5 || inc > 0)
                        map.w += inc * 2;
                    break;
                case 1:
                    if (map.h > 5 || inc > 0)
                        map.h += inc * 2;
                    break;
                case 2:
                    if (turns > 1 || inc > 0)
                        turns += inc;
                    break;
                
            }
            inc = 0;
        }

        SDL_RenderClear(renderer);

        char *str = (char*) malloc(10*sizeof(char));

        print_text("Map width = ", (SDL_Rect) {200, 100}, 3, (SDL_Color) 
                  {255,255*(selection !=0),255*(selection!=0),255});
        sprintf(str, "%d", map.w);
        print_text(str, (SDL_Rect) {550, 100}, 3, (SDL_Color) 
                  {255,255*(selection !=0),255*(selection!=0),255});

        print_text("Map height = ", (SDL_Rect) {200, 200}, 3, (SDL_Color) 
                  {255,255*(selection !=1),255*(selection!=1),255}); 
        sprintf(str, "%d", map.h);
        print_text(str, (SDL_Rect) {550, 200}, 3, (SDL_Color) 
                  {255,255*(selection !=1),255*(selection!=1),255});
        
        print_text("Player turns = ", (SDL_Rect) {200, 300}, 3, (SDL_Color) 
                  {255,255*(selection !=2),255*(selection!=2),255});
        sprintf(str, "%d", turns);
        print_text(str, (SDL_Rect) {550, 300}, 3, (SDL_Color) 
                  {255,255*(selection !=2),255*(selection!=2),255});
        
        free(str);

        print_text("Back", (SDL_Rect) {200, 450}, 3, (SDL_Color) 
                  {255,255*(selection !=3),255*(selection!=3),255});

        SDL_RenderPresent(renderer);
    }
}
