#ifndef BOMB_H
#define BOMB_H

#include <SDL2/SDL.h>

struct bomb {
    int x;
    int y;
    int time;
};

void explode_bomb(struct bomb *bomb);

#endif
