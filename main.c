#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "gamelogic.h"

// compilar con make

// variables globales para ser usadas en otros archivos.
TTF_Font *font = NULL;
SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;
int running = 1, options = 0, game = 0, turns = 300;


int main(int argc, char *argv[]) {
    
    if (init() == EXIT_FAILURE) // definido en utils.
        return EXIT_FAILURE;

    srand(time(NULL)); // se cambia la semilla.

    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    int selection = 0;

    while (running) {

        while (SDL_PollEvent(&event)) {
            // si se presiona la x de la ventana se cierra el programa.
            if (event.type == SDL_QUIT)
                running = 0;
            if (event.type == SDL_KEYDOWN)
                switch (event.key.keysym.sym) {
                    case SDLK_DOWN:
                        if (selection < 2)
                            selection++;
                        break;
                    case SDLK_UP:
                        if (selection > 0)
                            selection--;
                        break;
                    case SDLK_RETURN:
                    case SDLK_z:
                        switch (selection) {
                            case 0:
                                game = 1;
                                break;
                            case 1:
                                options = 1;
                                break;
                            case 2:
                                running = 0;
                                break;
                        }
                        break;
                }
        }
        
        SDL_RenderClear(renderer);

        // funcion print_text definida en utils.
        print_text("BOMBERMAN", (SDL_Rect) {100, 100}, 6, (SDL_Color) {32, 255, 255, 255});
        print_text("Start Game", (SDL_Rect) {300, 332}, 2, (SDL_Color) {255, 255*(selection!=0), 255*(selection!=0), 255});
        print_text("Options", (SDL_Rect) {300, 364}, 2, (SDL_Color) {255, 255*(selection!=1), 255*(selection!=1), 255});
        print_text("Exit", (SDL_Rect) {300, 396}, 2, (SDL_Color) {255, 255*(selection!=2), 255*(selection!=2), 255});

        SDL_RenderPresent(renderer);

        if (options)
            options_menu(); // menu de opciones. definido en gamelogic.

        if (game)
            run_game(); // se inicia el juego. definido en gamelogic.
        
    }

    quit();
    return EXIT_SUCCESS;
}
