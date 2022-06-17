#ifndef ENTITY_H
#define ENTITY_H

#include <wchar.h>
#include "opaque_types.h"

enum Facing { FACING_NORTH, FACING_SOUTH, FACING_EAST, FACING_WEST};

typedef struct {
    struct Position _position;
    struct Position _previous_position;
    enum   Facing   _facing_direction;
    enum   Color    _color;
} Entity;

typedef struct {
    Entity  *_self;
    wchar_t _aspects[4];
} Pacman;

typedef struct {
    Entity  *_self;
    wchar_t _aspect;
} Ghost;

Pacman  *pacman_init(Map *const map);
Ghost   *ghost_init(Map *const map, const uint8_t id);

#endif