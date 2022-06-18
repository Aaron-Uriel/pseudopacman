#include "map.h"
#include "entity.h"

void main_game() {
    Map *const map = map_init();
    Pacman *const pacman = pacman_init(map);
    Ghost *ghosts[ENTITY_LIMIT - 1];
    for (enum EntityID id = ENTITY_BLINKY; id < ENTITY_LIMIT; id++) {
        ghosts[id] = ghost_init(map, id);
    }
}