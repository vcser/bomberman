#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include <SDL2/SDL_render.h>
struct map {
    int w;
    int h;
    char **map;
    SDL_Texture *texture;
};

struct bomb {
    int x;
    int y;
    int time;
    SDL_Texture *texture;
};

struct player {
    int x;
    int y;
    int speed;
    SDL_Texture *texture;
    struct bomb *bombs;
};

void run_game();

void options_menu();

char **create_map(int map_h, int map_w);

void player_collisions(struct player *player);

#endif
