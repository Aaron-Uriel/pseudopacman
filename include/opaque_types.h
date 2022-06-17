#ifndef OPAQUE_TYPES_H
#define OPAQUE_TYPES_H

#include "types.h"

enum Color {COLOR_YELLOW, COLOR_COLOR_RED, COLOR_WHITE, COLOR_CYAN, COLOR_BLUE};

struct Position {
    uint8_t y;
    uint8_t x;
};

struct Position position_set_yx(struct Position *position, const uint8_t y, const uint8_t x);

#endif
