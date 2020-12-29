#include <SDL2/SDL.h>

void camera_present(struct player *player, struct map *map);

SDL_Texture *update_map_texture(const struct map *map);