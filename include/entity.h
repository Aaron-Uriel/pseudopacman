#ifndef ENTITY_H
#define ENTITY_H

#include <wchar.h>
#include "utilities.h"

enum Facing { FACING_NORTH, FACING_SOUTH, FACING_EAST, FACING_WEST};

typedef struct {
    struct Position _position;
    struct Position _previous_position;
    enum   Facing   _facing_direction;
    enum   EntityID _id;
} _Entity;

typedef struct {
    _Entity  *_self;
    wchar_t  _aspects[4];
} Pacman;

typedef struct {
    _Entity  *_self;
    wchar_t  _aspect;
} Ghost;

Pacman  *pacman_init(Map *const map);
Ghost   *ghost_init(Map *const map, const enum EntityID id);

#endif