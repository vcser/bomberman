#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include "utils.h"
#include "gamelogic.h"
#include <stdio.h>
#include <string.h>

void camera_present(struct player *player, struct map *map) {
    SDL_Rect player_on_screen = {WINDOW_WIDTH/2/*-22*3/2*/,WINDOW_HEIGHT/2/*-16*3/2*/, 16*3,16*3};
    SDL_Rect camera = {0,0,17*16,13*16};
    SDL_Rect dest_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    if (map->w < 17) {
        dest_rect.x = 17*16*3 - map->w*16*3;
        dest_rect.w = map->w*16*3;
    }
    if (map->h < 13) {
        dest_rect.y = 13*16*3 - map->h*16*3;
        dest_rect.h = map->h*16*3;

    }

    camera.x = player->x*16 - (float)camera.w/2;
    camera.y = player->y*16 - (float)camera.h/2;

    if (camera.x < 0) {
        player_on_screen.x += 3*camera.x;
        camera.x = 0;
    }
    if (camera.y < 0) {
        player_on_screen.y += 3*camera.y;
        camera.y = 0;
    }
    if (camera.x > map->w*16 - camera.w) {
        player_on_screen.x += 3*(camera.x + camera.w - map->w*16);
        camera.x = map->w*16 - camera.w;
    }
    if (camera.y > map->h*16 - camera.h) {
        player_on_screen.y += 3*(camera.y + camera.h - map->h*16);
        camera.y = map->h*16 - camera.h;
    }

    SDL_RenderCopy(renderer, map->texture, &camera, &dest_rect);
    SDL_RenderCopy(renderer, player->texture, NULL, &player_on_screen);
}

SDL_Texture *update_map_texture(const struct map *map) {
    SDL_Texture *map_texture;
    SDL_Surface *map_surface;
    SDL_Surface *hard_block_surface = IMG_Load("assets/hard_block.png");
    SDL_Surface *soft_block_surface = IMG_Load("assets/soft_block.png");
    SDL_Surface *bomb_surface = IMG_Load("assets/bomb.png");
    int spacing = 0;
    //int block_scale = 3;
    //if (map->w >= 17 && map->h >= 13)
    map_surface = SDL_CreateRGBSurfaceWithFormat(0, map->w*hard_block_surface->w, map->h*hard_block_surface->h, 32, hard_block_surface->format->format);
    //else
    //    map_surface = SDL_CreateRGBSurfaceWithFormat(0, 17*hard_block_surface->w, 13*hard_block_surface->h, 32, hard_block_surface->format->format);

    SDL_Rect dst_rect = {0,0,hard_block_surface->w,hard_block_surface->h};
    
    for (int i = 0; i < map->h; i++) {
        dst_rect.y = i*hard_block_surface->h;
        for (int j = 0; j < map->w; j++) {
            dst_rect.x = j*hard_block_surface->w;
            if (map->map[i][j] == '#')
                SDL_BlitSurface(hard_block_surface, NULL, map_surface, &dst_rect);
            else if (map->map[i][j] == '%')
                SDL_BlitSurface(soft_block_surface, NULL, map_surface, &dst_rect);
            else if (map->map[i][j] == 'Q')
                SDL_BlitSurface(bomb_surface, NULL, map_surface, &dst_rect);
        }
    }

    map_texture = SDL_CreateTextureFromSurface(renderer, map_surface);
    SDL_FreeSurface(bomb_surface);
    SDL_FreeSurface(map_surface);
    SDL_FreeSurface(hard_block_surface);
    SDL_FreeSurface(soft_block_surface);
    return map_texture;
}
