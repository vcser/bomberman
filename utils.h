#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 816
#define WINDOW_HEIGHT 624

// se imprime texto en pantalla en la posicion y color entregados.
void print_text(const char *text, SDL_Rect rect, float scale, SDL_Color color);

// se renderiza una imagen en la posicion y con la escala entregadas.
void render_image(SDL_Texture *image_texture, SDL_Rect rect, float scale);

// cargar imagenes png y obtener un puntero a la textura.
SDL_Texture *load_image(const char *name);

double get_delta_time();

// inicializacion y cierre de SDL.
int init(void);
void quit(void);

// variables externas para usar en otros archivos. Todas pertenecen a main.
extern TTF_Font *font;
extern SDL_Renderer *renderer;
extern SDL_Window *window;

#endif
