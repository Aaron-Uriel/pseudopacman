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

void draw_window_borders(WINDOW *const window);

#endif
