#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include "entity.h"

Entity *entity_init(Map *const map, const enum EntityID id, const wchar_t aspect[3]) {
    Entity *const new_entity = malloc(sizeof(*new_entity));

    struct Position new_entity_position = map_search_for_wchar(map, (id + L'0'), REPLACE_WCH_TRUE);

    new_entity->_position = new_entity_position;
    new_entity->_previous_position = new_entity_position;
    new_entity->facing_direction = FACING_EAST;
    new_entity->id = id;
    new_entity->color = id + 1;
    wcscpy(new_entity->aspect, aspect);

    return new_entity;
}

struct Position entity_get_position(const Entity *const entity, const enum PositionType position_type) {
    struct Position position;

    switch (position_type) {
        case POSITION_TYPE_CURRENT:
            position = entity->_position;
            break;
        case POSITION_TYPE_PREVIOUS:
            position = entity->_previous_position;
    }
    return position;
}

void entity_new_relative_position(Entity *const entity, const Map *const map, const int8_t delta_y, const int8_t delta_x) {
    entity->_previous_position = entity->_position;

    struct Resolution map_size = map_get_size(map);

    wchar_t test_wch;
    if (((entity->_position.y + delta_y) >= 0 && (entity->_position.y + delta_y) < map_size.length) && ((entity->_position.x + delta_x) >= 0 && (entity->_position.x + delta_x) < map_size.width)) {
        test_wch = *map_get_wchar_from_coords(map, entity->_position.y + delta_y, entity->_position.x + delta_x);
        if (test_wch == L' ') {
            entity->_position.y += delta_y;
            entity->_position.x += (delta_x * 2);
        }
    }
}