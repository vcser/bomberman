// la funcion devuelve el mapa como puntero de punteros (array 2d). se necesita el ancho y el largo del mapa. 
char** create_map(int map_h, int map_w);

// imprime el mapa en pantalla. se necesita el mapa a imprimir y la porcion de mapa que se quiere mostrar.
void print_map(char **map, int map_w, int map_h);