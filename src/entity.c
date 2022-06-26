#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <assert.h>
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

uint8_t entity_new_relative_position(Entity *const entity, const Map *const map, const int8_t delta_y, const int8_t delta_x) {
    entity->_previous_position = entity->_position;

    struct Resolution map_size = map_get_size(map);

    wchar_t test_wch;
    if (((entity->_position.y + delta_y) >= 0 && (entity->_position.y + delta_y) < map_size.length) && ((entity->_position.x + delta_x) >= 0 && (entity->_position.x + delta_x) < map_size.width)) {
        test_wch = *map_get_wchar_from_coords(map, entity->_position.y + delta_y, entity->_position.x + delta_x);
        if (test_wch == L' ') {
            entity->_position.y += delta_y;
            entity->_position.x += delta_x;
            return 0;
        }
    }
    return 1;
}

bool is_cardinal_point_available(const Entity *const entity, const Map *const map, const enum Cardinal cardinal_point) {
    struct Position test_position = entity_get_position(entity, POSITION_TYPE_CURRENT);
    switch (cardinal_point) {
        case CARDINAL_NORTH: test_position.y -= 1; break;
        case CARDINAL_SOUTH: test_position.y += 1; break;
        case CARDINAL_EAST: test_position.x += 1; break;
        case CARDINAL_WEST: test_position.x -= 1; break;
        default: assert(false && "Default case should not be possible");
    }

    wchar_t test_wch = *map_get_wchar_from_coords(map, test_position.y, test_position.x);
    return (test_wch == ' ')? true: false;
}

struct AvailablePaths entity_get_available_paths(const Entity *const entity, const Map *const map) {
    struct AvailablePaths available_paths = { 0 };
    wchar_t *test_wch;
    test_wch = map_get_wchar_from_coords(map, entity->_position.y - 1, entity->_position.x);
    if (*test_wch == ' ') {
        available_paths.path_array[CARDINAL_NORTH] = true;
        available_paths.free_paths_count++;
    }
    test_wch = map_get_wchar_from_coords(map, entity->_position.y + 1, entity->_position.x);
    if (*test_wch == ' ') {
        available_paths.path_array[CARDINAL_SOUTH] = true;
        available_paths.free_paths_count++;
    }
    test_wch = map_get_wchar_from_coords(map, entity->_position.y, entity->_position.x + 2);
    if (*test_wch == ' ') {
        available_paths.path_array[CARDINAL_EAST] = true;
        available_paths.free_paths_count++;
    }
    test_wch = map_get_wchar_from_coords(map, entity->_position.y, entity->_position.x - 2);
    if (*test_wch == ' ') {
        available_paths.path_array[CARDINAL_WEST] = true;
        available_paths.free_paths_count++;
    }

    return available_paths;
}

void entity_perform(Entity *const entity, const Map *const map) {
    const struct AvailablePaths available_paths = entity_get_available_paths(entity, map);

    const bool entity_is_blocked = (available_paths.path_array[entity->facing_direction] == false);
    if (entity_is_blocked) {
        enum Facing new_facing_direction;
        do {
            new_facing_direction = rand() % FACING_LIMIT;
        } while (available_paths.path_array[new_facing_direction] == false);

        entity->facing_direction = new_facing_direction;
        
        return;
    } else
    if (available_paths.free_paths_count > 2) {
        const bool entity_decides_to_change_its_path = ((rand() % 10) >= 7)? true: false; // El chiste es que haya un 30% de posibilidades de que decida cambiar de camino
        if (entity_decides_to_change_its_path) {
            switch (entity->facing_direction) {
                case FACING_NORTH: case FACING_SOUTH: entity->facing_direction = rand_one_or_the_other(FACING_EAST, FACING_WEST); break;
                case FACING_EAST:  case FACING_WEST:  entity->facing_direction = rand_one_or_the_other(FACING_NORTH, FACING_SOUTH); break;
                default: assert(false && "Default case should not be possible");
            }
        }
    }
    return;
}