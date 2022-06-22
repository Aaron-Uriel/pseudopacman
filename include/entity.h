#ifndef ENTITY_H
#define ENTITY_H

#include <wchar.h>
#include "utilities.h"
#include "map.h"

enum Facing   { FACING_NORTH, FACING_SOUTH, FACING_EAST, FACING_WEST};
enum EntityID { ENTITY_PACMAN, ENTITY_BLINKY, ENTITY_PINKY, ENTITY_INKY, ENTITY_CLYDE, ENTITY_LIMIT };
enum Color    { COLOR_PACMAN = 1, COLOR_BLINKY, COLOR_PINKY, COLOR_INKY, COLOR_CLYDE, COLOR_LIMIT};
enum PositionType { POSITION_TYPE_CURRENT, POSITION_TYPE_PREVIOUS };

struct Movement {
    enum { AXIS_Y, AXIS_X } axis;
    enum { CHANGE_NEGATIVE, CHANGE_POSITIVE} change;
};

typedef struct {
    struct  Position _position;
    struct  Position _previous_position;
    enum    Facing   facing_direction;
    enum    EntityID id;
    enum    Color    color;
    struct  Movement movement;

    wchar_t aspect[3];
} Entity;

Entity *entity_init(Map *const map, const enum EntityID id, const wchar_t aspect[3]);
struct Position entity_get_position(const Entity *const entity, const enum PositionType position_type);
void entity_new_relative_position(Entity *const entity, const Map *const map, const int8_t delta_y, const int8_t delta_x);

#endif