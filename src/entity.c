#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include "entity.h"

Entity *entity_init(Map *const map, const enum EntityID id, const wchar_t aspect[3]) {
    Entity *const new_entity = malloc(sizeof(*new_entity));

    struct Position new_entity_position = map_search_for_wchar(map, (id + L'0'), REPLACE_WCH_TRUE, KIND_REAL);

    new_entity->_position = new_entity_position;
    new_entity->_previous_position = new_entity_position;
    new_entity->id = id;
    new_entity->color = id + 1;
    wcscpy(new_entity->aspect, aspect);

    return new_entity;
}

struct Position entity_get_position(const Entity *const entity, enum Kind position_kind) {
    struct Position position;
    switch (position_kind) {
        case KIND_ABSTRACT:
            position.y = entity->_position.y;
            position.x = entity->_position.x / 2;
            break;
        case KIND_REAL:
            position = entity->_position;
    }
    
    return position;
}