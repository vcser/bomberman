#include "bomb.h"
#include "utils.h"
#include "map.h"
#include "player.h"
#include "graphics.h"
#include "powerups.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

static SDL_Texture *explosion_frames[8] = {NULL};
static SDL_Texture *bomb_texture;
extern struct map map;
extern struct player player;
extern int game;

int continue_explosion(struct bomb *bomb, int x, int y) {
    if (map.map[y][x] == '#')
        return 0;

    if (x == player.x && y == player.y) {
        game = 0;
        return 0;
    }

    
    if (x == map.exit.x && y == map.exit.y) {
        map.map[y][x] = '*';
        return 0;
    }
    
    if(map.map[y][x] == 'P') {
        if (rand()%2) 
            map.map[y][x] = 'F';
        else
            map.map[y][x] = 'B';
        return 0;
    }

    if (map.map[y][x] == '%' || map.map[y][x] == 'F' || map.map[y][x] == 'B') {
        map.map[y][x] = ' ';
        return 0;
    }
    
    if (map.map[y][x] == ' ')
        return 1;
    
    return 0;
}

void explode_bomb(struct bomb *bomb) {
    // si la bomba alcanza un bloque blando, se destruye.
    for (int i = 1; i <= player.bomb_length; i++) {
        if (!continue_explosion(bomb, bomb->x-i, bomb->y))
            break;
    }

    for (int i = 1; i <= player.bomb_length; i++) {
        if (!continue_explosion(bomb, bomb->x+i, bomb->y))
            break;
    }

    for (int i = 1; i <= player.bomb_length; i++) {
        if (!continue_explosion(bomb, bomb->x, bomb->y-i))
            break;
    }

    for (int i = 1; i <= player.bomb_length; i++) {;
        if (!continue_explosion(bomb, bomb->x, bomb->y+i))
            break;
    }

    if (bomb->x == player.x && bomb->y == player.y)
        game = 0;
        
    
    // se elimina el caracter 'Q' del mapa y se deja el tiempo de la bomba en -1.
    map.map[bomb->y][bomb->x] = ' ';
    bomb->time--;
    //SDL_DestroyTexture(map.texture);
    update_map_texture(&map);
}

void init_bombs() {
    bomb_texture = load_image("assets/bomb->png");
}

void clean_bombs() {
    SDL_DestroyTexture(bomb_texture);
}