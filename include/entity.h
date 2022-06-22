#ifndef ENTITY_H
#define ENTITY_H

#include <wchar.h>
#include "utilities.h"
#include "map.h"

enum Facing   { FACING_NORTH, FACING_SOUTH, FACING_EAST, FACING_WEST};
enum EntityID { ENTITY_PACMAN, ENTITY_BLINKY, ENTITY_PINKY, ENTITY_INKY, ENTITY_CLYDE, ENTITY_LIMIT };
enum Color    { COLOR_PACMAN = 1, COLOR_BLINKY, COLOR_PINKY, COLOR_INKY, COLOR_CLYDE, COLOR_LIMIT};

typedef struct {
    struct  Position _position;
    struct  Position _previous_position;
    enum    Facing   _facing_direction;
    enum    EntityID id;
    enum    Color    color;

    wchar_t aspect[3];
} Entity;

Entity *entity_init(Map *const map, const enum EntityID id, const wchar_t aspect[3]);
struct Position entity_get_position(const Entity *const entity, enum Kind position_kind);

#endif