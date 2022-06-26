#ifndef ENTITY_H
#define ENTITY_H

#include <wchar.h>
#include "utilities.h"
#include "map.h"

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
    enum    Facing   facing_direction;
    enum    EntityID id;
    enum    Color    color;

    wchar_t aspect[3];
} Entity;

Entity *entity_init(Map *const map, const enum EntityID id, const wchar_t aspect[3]);
struct Position entity_get_position(const Entity *const entity, const enum PositionType position_type);
uint8_t entity_new_relative_position(Entity *const entity, const Map *const map, const int8_t delta_y, const int8_t delta_x);

bool is_cardinal_point_available(const Entity *const entity, const Map *const map, const enum Cardinal cardinal_point);
struct AvailablePaths entity_get_available_paths(const Entity *const entity, const Map *const map);
void entity_perform(Entity *const entity, const Map *const map);

#endif