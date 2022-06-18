#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <ctype.h>
#include "map.h"
#include "entity.h"

_Entity *entity_init(Map *const map, const enum EntityID id) {
    _Entity *const new_entity = malloc(sizeof(*new_entity));

    struct Position new_entity_position = map_search_for_wchar(map, (id + L'0'), true);

    new_entity->_position = new_entity_position;
    new_entity->_previous_position = new_entity_position;
    new_entity->_id = id;

    return new_entity;
}

Pacman *pacman_init(Map *const map) {
    Pacman *new_pacman = malloc(sizeof(*new_pacman));
    new_pacman->_self = entity_init(map, ENTITY_PACMAN);
    
    new_pacman->_aspects[FACING_NORTH] = 0x15E4;
    new_pacman->_aspects[FACING_SOUTH] = 0x15E4;
    new_pacman->_aspects[FACING_EAST]  = 0X15E7;
    new_pacman->_aspects[FACING_WEST]  = 0X15E7;

    return new_pacman;
}

Ghost *ghost_init(Map *const map, const enum EntityID id) {
    Ghost *new_ghost = malloc(sizeof(*new_ghost));
    new_ghost->_self = entity_init(map, id);

    new_ghost->_aspect = 0x15E4;

    return new_ghost;
}