#ifndef UTILITIES_H
#define UTILITIES_H

#include <ncurses.h>
#include "types.h"

struct Resolution {
    uint8_t length;
    uint8_t width;
};
extern struct Resolution terminal_resolution;

struct Position {
    uint8_t y;
    uint8_t x;
};

enum EntityID { ENTITY_PACMAN, ENTITY_BLINKY, ENTITY_PINKY, ENTITY_INKY, ENTITY_CLYDE, ENTITY_LIMIT };

void draw_window_borders(WINDOW *const window);

#endif
