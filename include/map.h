#ifndef MAP_H
#define MAP_H

#include <wchar.h>
#include <stdbool.h>
#include "types.h"
#include "opaque_types.h"

typedef struct {
    bool is_initialized;
    uint8_t _length;
    uint8_t _width;
    wchar_t _raw_map[];
} Map;

Map *map_init();
struct Position map_search_for_wchar(const Map *const map, wchar_t wch);


#endif