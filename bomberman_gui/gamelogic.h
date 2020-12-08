struct map {
    int w;
    int h;
    char **map;
};

void run_game();

void options_menu();

char **create_map(int map_h, int map_w);


void render_map(struct map *map); // se debe cambiar esta funcion.
