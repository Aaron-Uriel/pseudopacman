#ifndef WORLD_H
#define WORLD_H

#include <wchar.h>
#include <stdbool.h>
#include "types.h"
#include "utilities.h"

enum Consts {
    FILE_MAX_WIDTH = 80,
    CELL_AESTHETIC_WIDTH = 2
};

enum Cardinal { CARDINAL_NORTH, CARDINAL_SOUTH, CARDINAL_EAST, CARDINAL_WEST, CARDINAL_LIMIT};
enum Facing   { FACING_NORTH, FACING_SOUTH, FACING_EAST, FACING_WEST, FACING_LIMIT};
enum EntityID { ENTITY_PACMAN, ENTITY_BLINKY, ENTITY_PINKY, ENTITY_INKY, ENTITY_CLYDE, ENTITY_LIMIT };
enum Color    { COLOR_PACMAN = 1, COLOR_BLINKY, COLOR_PINKY, COLOR_INKY, COLOR_CLYDE, COLOR_LIMIT };
enum PositionType { POSITION_TYPE_CURRENT, POSITION_TYPE_PREVIOUS };

struct AvailablePaths {
    uint8_t free_paths_count;
    bool path_array[CARDINAL_LIMIT];
};

typedef struct {
    struct  Position _position;
    struct  Position _previous_position;
    uint8_t _entity_holder_position;
    enum    Facing   facing_direction;
    enum    EntityID id;
    enum    Color    color;

    wchar_t aspect;
} Entity;

struct Cell {
    enum { CELL_TYPE_WALL, CELL_TYPE_ENTITY_PATHWAY } type;
    wchar_t content;
};

typedef struct {
    bool is_initialized;
    struct Resolution _size;
    struct Cell _cell_map[];
} Map;


Entity *entity_init(Map *const map, const enum EntityID id, const wchar_t aspect);
struct Position entity_get_position(const Entity *const entity, const enum PositionType position_type);
uint8_t entity_set_new_relative_position(Entity *const entity, const Map *const map, const int8_t delta_y, const int8_t delta_x);
struct AvailablePaths entity_get_available_paths(const Entity *const entity, const Map *const map);
void entity_perform(Entity *const entity, const Map *const map);

Map *map_init();
struct Resolution map_get_size(const Map *const map);
void map_draw(const Map *const map, WINDOW *const window);


#endif