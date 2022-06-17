#include <locale.h>
#include "map.h"
#include "entity.h"
 
enum Consts {
    GHOSTS_LIMIT = 4
};


int main() {
    setlocale(LC_CTYPE, "");

    Map *const map = map_init();
    Pacman *const pacman = pacman_init(map);
    Ghost *ghosts[GHOSTS_LIMIT];
    for (uint8_t i = 1; i < GHOSTS_LIMIT; i++) {
        ghosts[i] = ghost_init(map, i);
    }
    return 0;
}