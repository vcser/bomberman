#include "utils.h"
#include "gamelogic.h"
#include <stdio.h>

extern int game, running, options;

void run_game() {
    // se inicializan las variables
    SDL_Texture *player = load_image("assets/player.png"); // textura del jugador
    
    // posicion inicial del jugador.
    float x = 100;
    float y = 100;
    float velocity = 2; // velocidad del jugador.

    // NOW y LAST son usados para calcular el delta time
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;

    // delay es usado como timer para imprimir los fps en el titulo de la ventana
    Uint32 delay = SDL_GetTicks() + 500;

    // se crea el mapa
    map.map = create_map(map.h, map.w);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    while (game) {

        // calculo de delta time y limitador de fps
        Uint32 start = SDL_GetTicks();
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = ((NOW - LAST)*100 / (double)SDL_GetPerformanceFrequency() );
        //printf("%f\n", deltaTime);

        // imprimir fps en el titulo de la ventana
        if (SDL_GetTicks() >= delay) {
            char *str = (char*) malloc(20*sizeof(char));
            snprintf(str, 20, "FPS = %.1f", 1/(deltaTime/100));
            SDL_SetWindowTitle(window, str);
            free(str);
            delay = SDL_GetTicks() + 500;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT){
                game = 0;
                running = 0;
            }
        
        // se renderiza el mapa y el jugador
        SDL_RenderClear(renderer);
        render_map(&map);
        render_image(player, (SDL_Rect) {x, y}, 3);
        SDL_RenderPresent(renderer);

        // se recibe el input del teclado
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        // se multiplica por deltaTime para tener un movimiento estable.
        if (state[SDL_SCANCODE_DOWN])
            y += velocity * deltaTime;
        else if (state[SDL_SCANCODE_UP])
            y -= velocity * deltaTime;
        else if (state[SDL_SCANCODE_RIGHT])
            x += velocity * deltaTime;
        else if (state[SDL_SCANCODE_LEFT])
            x -= velocity * deltaTime;
        if (state[SDL_SCANCODE_ESCAPE])
            game = 0;


        // aqui se calcula cuanto tiempo esperar para mantener un framerate de 60 fps
        Uint32 FrameTime = SDL_GetTicks() - start;
        // limitador de FPS.
        if (FrameTime < 1000/60)
            SDL_Delay(1000/60 - FrameTime);
    }

    //liberar memoria del mapa y terminar el juego.
    for (int i = 0; i < map.h; i++)
            free(*(map.map + i));
    free(map.map);
    SDL_SetWindowTitle(window, "BOMBERMAN");
    SDL_DestroyTexture(player);
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
    /*
     *     # # # # # # #
     *     #           #
     *     #   #   #   #
     *     #           #
     *     #   #   #   #
     *     #           #
     *     # # # # # # #
     *
     */
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
            // si el espacio esta vacio y rand() es par, se cambia por un bloqe blando.
            if (map[i][j] == ' ' && rand()%2)
                map[i][j] = '%';
    
    // esto es para asegurarse que la esquina donde aparece el jugador este vacia.
    map[1][1] = ' ';
    map[1][2] = ' ';
    map[2][1] = ' ';

    return map;
}

void render_map(struct map *map) {
    // esta funcion es muy ineficiente pero funciona.

    // se cargan las tecturas de los bloques del mapa.
    SDL_Texture *hard_block = load_image("assets/hard_block.png");
    SDL_Texture *soft_block = load_image("assets/soft_block.png");

    // tamaño del lado del cuadrado donde se dibujara el mapa.
    int drawing_rect_size = 550;

    // se calcula el tamaño de cada textura.
    // spacing es el alto y ancho que debe tener cada textura de bloque.
    float spacing = (float) drawing_rect_size/map->h;

    // esto es para asegurarse que el cuadrado de dibujo quede centrado
    SDL_Rect rect = {(float)(WINDOW_WIDTH-drawing_rect_size)/2-spacing,WINDOW_HEIGHT-drawing_rect_size-spacing};
    rect.w = spacing;
    rect.h = spacing;

    // se dibuja el mapa, cada bloque de tamaño "spacing".
    for (int i = 0; i < map->h; i++) {
        rect.y += spacing;
        rect.x = (float)(WINDOW_WIDTH-drawing_rect_size)/2-spacing;
        for (int j = 0; j < map->w; j++) {
            rect.x += spacing;
            if (map->map[i][j] == '#')
                SDL_RenderCopy(renderer, hard_block, NULL, &rect);
            else if (map->map[i][j] == '%')
                SDL_RenderCopy(renderer, soft_block, NULL, &rect);
        }
    }

    SDL_DestroyTexture(hard_block);
    SDL_DestroyTexture(soft_block);
}


// menu de opciones.
// esta funcion esta muy enredada.
void options_menu() {
    struct selection {
        int x;
        int y;
        int in_submenu;
    } selection;

    selection.x = 0;
    selection.y = 0;
    selection.in_submenu = 0;

    int *submenus[1][2];
    submenus[0][0] = &map.w;
    submenus[0][1] = &map.h;

    while (options) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT){
                options = 0;
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        if (selection.in_submenu)
                            selection.in_submenu = !selection.in_submenu;
                        else
                            options = 0;
                        break;
                    case SDLK_UP:
                        if (!selection.in_submenu) {
                            if (selection.y > 0)
                                selection.y--;
                        } else 
                            (*submenus[selection.y][selection.x])++;
                        break;
                    case SDLK_DOWN:
                        if (!selection.in_submenu) {
                            if (selection.y < 1)
                                selection.y++;
                        } else
                            (*submenus[selection.y][selection.x])--;
                        break;
                    case SDLK_RIGHT:
                        if (selection.in_submenu && selection.x < 1)
                            selection.x++;
                        break;
                    case SDLK_LEFT:
                        if (selection.in_submenu && selection.x > 0)
                            selection.x--;
                        break;
                    case SDLK_RETURN:
                        selection.in_submenu = !selection.in_submenu;
                        switch (selection.y) {
                            case 0: break;
                            case 1: options = 0; break;
                        }
                        break;
                }
            }
        }

        SDL_RenderClear(renderer);

        print_text("Map size = ", (SDL_Rect) {200, 300}, 3, (SDL_Color) 
                  {255,255*(selection.y!=0 || selection.in_submenu),255*(selection.y!=0 || selection.in_submenu),255});

        char *str = (char*) malloc(10*sizeof(char));
        sprintf(str, "%d", map.w);
        print_text(str, (SDL_Rect) {440, 300}, 3, (SDL_Color) 
                  {255,255*(selection.x!=0 || selection.y!=0 || !selection.in_submenu),255*(selection.x!=0 || selection.y!=0 || !selection.in_submenu),255});

        print_text("X", (SDL_Rect) {500,300}, 3, (SDL_Color) {255,255,255,255});

        sprintf(str, "%d", map.h);
        print_text(str, (SDL_Rect) {540, 300}, 3, (SDL_Color) 
                  {255,255*(selection.x!=1 || selection.y!=0 || !selection.in_submenu),255*(selection.x!=1 || selection.y!=0 || !selection.in_submenu),255});
        free(str);

        print_text("Back", (SDL_Rect) {200, 350}, 3, (SDL_Color) 
                  {255,255*(selection.y!=1),255*(selection.y!=1),255});

        SDL_RenderPresent(renderer);
    }
}