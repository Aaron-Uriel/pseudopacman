#ifndef MAP_H
#define MAP_H

#include <wchar.h>
#include <stdbool.h>
#include "types.h"
#include "utilities.h"

typedef struct {
    bool is_initialized;
    struct Resolution _size;
    wchar_t _raw_map[];
} Map;

Map *map_init();
struct Resolution map_get_size(const Map *const map);
struct Position map_search_for_wchar(Map *const map, const wchar_t wch, const bool replace_wch);
void map_draw(const Map *const map, const WINDOW *const window);


#endif