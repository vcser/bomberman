#include "utils.h"
#include <SDL2/SDL_pixels.h>

void print_text(const char *text, SDL_Rect rect, float scale, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect.w = surface->w * scale;
    rect.h = surface->h * scale;
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render_image(SDL_Texture *image_texture, SDL_Rect rect, float scale) {
    SDL_QueryTexture(image_texture, NULL, NULL, &(rect.w), &(rect.h));
    rect.w *= scale;
    rect.h *= scale;
    SDL_RenderCopy(renderer, image_texture, NULL, &rect);
}

SDL_Texture *load_image(const char *name) {
    SDL_Surface *image_surface = IMG_Load(name);
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);
    return image_texture;
}

int init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error al iniciar SDL\n");
        return EXIT_FAILURE;
    }

    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    font = TTF_OpenFont("assets/Roboto-Regular.ttf", 16);
    if (!font)
        printf("Error al abrir font\n");

    window = SDL_CreateWindow("BOMBERMAN", 
                              SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 
                              WINDOW_WIDTH, 
                              WINDOW_HEIGHT, 
                              0);
    if (!window) {
        printf("Error al crear la ventana\n");
        SDL_Quit();
        return EXIT_FAILURE;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Error al crear el renderer\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    return 0;
}

void quit() {
    IMG_Quit();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

double get_delta_time() {
    static Uint64 LAST = 0;
    double delta_time = 0;
    Uint64 NOW = SDL_GetPerformanceCounter();
    printf("NOW = %ld\n", NOW);
    printf("LAST = %ld\n", LAST);

    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();
    delta_time = ((NOW - LAST)*100 / (double)SDL_GetPerformanceFrequency());

    return delta_time;
}
