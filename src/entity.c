#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <ctype.h>
#include "map.h"
#include "entity.h"

Entity *entity_init(Map *const map, const uint8_t id) {
    Entity *const new_entity = malloc(sizeof(*new_entity));

    struct Position new_entity_position = map_search_for_wchar(map, (id + L'0'));
    printf("Posición: %d %d", new_entity_position.y, new_entity_position.x);

    return new_entity;
}

Pacman *pacman_init(Map *const map) {
    Pacman *new_pacman = malloc(sizeof(*new_pacman));
    new_pacman->_self = entity_init(map, 0);
    
    new_pacman->_aspects[FACING_NORTH] = 0x15E4;
    new_pacman->_aspects[FACING_SOUTH] = 0x15E4;
    new_pacman->_aspects[FACING_EAST]  = 0X15E7;
    new_pacman->_aspects[FACING_WEST]  = 0X15E7;

    return new_pacman;
}

Ghost *ghost_init(Map *const map, const uint8_t id) {
    Ghost *new_ghost = malloc(sizeof(*new_ghost));
    new_ghost->_self = entity_init(map, id);

    new_ghost->_aspect = 0x15E4;
}