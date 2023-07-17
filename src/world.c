#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <wchar.h>

#include "world.h"


// Funciones usadas solo en este archivo

struct Cell *map_get_cell_from_coords(const Map *const map, const uint8_t y, const uint8_t x) {
    assert(map->is_initialized == true);

    struct Cell (*map_matrix)[map->_size.width] = (struct Cell(*)[map->_size.width]) map->_cell_map;
    return &map_matrix[y][x];
}

struct Position map_find_initial_entity_position_with_id(Map *const map, const uint8_t id) {
    assert(map->is_initialized == true);

    const wchar_t id_wch = digit_to_char(id);

    struct Position initial_entity_position = { 0 };
    struct Cell *test_cell;
    for (uint8_t row = 0; row < map->_size.length; row++) {
        for (uint8_t column = 0; column < map->_size.width; column++) {
            test_cell = map_get_cell_from_coords(map, row, column);

            if (test_cell->type != CELL_TYPE_WALL) { continue; }

            if (test_cell->content == id_wch) {
                initial_entity_position.y = row;
                initial_entity_position.x = column;

                // Cambiamos el tipo de la casilla y la reiniciamos ya que encontramos el id de la entidad
                test_cell->type = CELL_TYPE_ENTITY_PATHWAY;
                test_cell->content = ' ';
                goto loop_exit;
            }
        }
    }
    loop_exit: ;

    return initial_entity_position;
}

// Fin de funciones solo usadas en este archivo


Entity *entity_init(Map *const map, const enum EntityID id, const wchar_t aspect) {
    Entity *const new_entity = malloc(sizeof(*new_entity));

    struct Position new_entity_position = map_find_initial_entity_position_with_id(map, id);

    new_entity->_position = new_entity_position;
    new_entity->_previous_position = new_entity_position;
    new_entity->facing_direction = FACING_EAST;
    new_entity->id = id;
    new_entity->color = id + 1;
    new_entity->aspect = aspect;

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

uint8_t entity_set_new_relative_position(Entity *const entity, const Map *const map, const int8_t delta_y, const int8_t delta_x) {
    entity->_previous_position = entity->_position;

    struct Resolution map_size = map_get_size(map);

    struct Cell *test_cell;
    if (((entity->_position.y + delta_y) >= 0 && (entity->_position.y + delta_y) < map_size.length) && ((entity->_position.x + delta_x) >= 0 && (entity->_position.x + delta_x) < map_size.width)) {
        test_cell = map_get_cell_from_coords(map, entity->_position.y + delta_y, entity->_position.x + delta_x);
        if (test_cell->type == CELL_TYPE_ENTITY_PATHWAY) {
            entity->_position.y += delta_y;
            entity->_position.x += delta_x;

            return 0;
        }
    }
    return 1;
}

struct AvailablePaths entity_get_available_paths(const Entity *const entity, const Map *const map) {
    struct AvailablePaths available_paths = { 0 };
    struct Cell *test_cell;
    test_cell = map_get_cell_from_coords(map, entity->_position.y - 1, entity->_position.x);
    if (test_cell->type == CELL_TYPE_ENTITY_PATHWAY) {
        available_paths.path_array[CARDINAL_NORTH] = true;
        available_paths.free_paths_count++;
    }
    test_cell = map_get_cell_from_coords(map, entity->_position.y + 1, entity->_position.x);
    if (test_cell->type == CELL_TYPE_ENTITY_PATHWAY) {
        available_paths.path_array[CARDINAL_SOUTH] = true;
        available_paths.free_paths_count++;
    }
    test_cell = map_get_cell_from_coords(map, entity->_position.y, entity->_position.x + 1);
    if (test_cell->type == CELL_TYPE_ENTITY_PATHWAY) {
        available_paths.path_array[CARDINAL_EAST] = true;
        available_paths.free_paths_count++;
    }
    test_cell = map_get_cell_from_coords(map, entity->_position.y, entity->_position.x - 1);
    if (test_cell->type == CELL_TYPE_ENTITY_PATHWAY) {
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


Map *map_init() {
    FILE *map_file = fopen("./files/map.txt", "r");
    assert(map_file != NULL);

    wchar_t character;
    uint8_t row, column;
    for (row = 1; true; row++) {
        for (column = 1; true; column++) {
            character = fgetwc(map_file);
            switch (character) {
                case '\n': goto column_end;
                case EOF:  goto file_end;
            }
        }
        column_end: ;
    }
    file_end: ;
    rewind(map_file);

    const uint8_t map_length = row;
    const uint8_t map_width  = column;
    Map *const new_map = calloc(1, sizeof(*new_map) + sizeof(struct Cell[map_length][map_width]));
    new_map->_size.length = map_length;
    new_map->_size.width = map_width;

    struct Cell (*map_matrix)[map_width] = (struct Cell(*)[map_width]) new_map->_cell_map;


    wchar_t current_line[map_width + 1];
    struct Cell *current_cell;
    for (row = 0; row < map_length; row++) {
        fgetws(current_line, map_width + 1, map_file);

        for (column = 0; column < wcslen(current_line); column++) {
            current_cell = &map_matrix[row][column];

            current_cell->content = current_line[column];
            switch (current_cell->content) {
                case ' ':
                    current_cell->type = CELL_TYPE_ENTITY_PATHWAY;
                break;
                case '\n': case '\0':
                    continue;
                default: 
                    current_cell->type = CELL_TYPE_WALL;
            }
        }
    }
    new_map->is_initialized = true;

    return new_map;
}

struct Resolution map_get_size(const Map *const map) {
    assert(map->is_initialized == true);

    return map->_size;
}

void map_draw(const Map *const map, WINDOW *const window) {
    assert(map->is_initialized == true);

    const struct Resolution map_size = map_get_size(map);
    struct Resolution window_size;
    getmaxyx(window, window_size.length, window_size.width);
    assert((map_size.length <= window_size.length) && (map_size.width <= window_size.width));

    struct Cell *current_cell;
    for (uint8_t row = 0; row < map_size.length; row++) {
        wmove(window, row, 0);
        for (uint8_t column = 0; column < map_size.width; column++) {
            current_cell = map_get_cell_from_coords(map, row, column);
            wprintw(window, "%lc%lc", current_cell->content, current_cell->content);
        }
    }
    
}
