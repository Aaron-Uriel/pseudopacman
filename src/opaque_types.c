#include "opaque_types.h"

struct Position position_set_yx(struct Position *const position, const uint8_t y, const uint8_t x) {
    position->y = y;
    position->x = x;
}