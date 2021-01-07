#include <stdlib.h>

// compilar con make

int init();
void mainloop();
void cleanup();

int main(int argc, char *argv[]) {
    
    if (init() == EXIT_FAILURE) // definido en utils.
        return EXIT_FAILURE;
    mainloop();
    cleanup();
    return EXIT_SUCCESS;
}
