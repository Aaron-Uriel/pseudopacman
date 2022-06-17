#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "map.h"

enum Consts {
    FILE_MAX_WIDTH = 80
};

Map *map_init() {
    FILE *map_file = fopen("/home/auriel/Documentos/pseudopacman/files/map.txt", "r");
    assert(map_file != NULL);

    wchar_t character;
    uint8_t row, column;
    for (row = 1; true; row++) {
        for (column = 1; true; column++) {
            character = fgetwc(map_file);
            switch (character) {
                case EOF:  goto file_end;
                case '\n': goto column_end;
            }
        }
        column_end:
    }
    file_end:
    rewind(map_file);

    const uint8_t map_lenght = row, map_width = column;
    Map *const new_map = calloc(1, sizeof(*new_map) + sizeof(wchar_t[map_lenght][map_width]));
    new_map->_length = map_lenght;
    new_map->_width  = map_width;

    wchar_t (*map_matrix)[map_width] = (wchar_t(*)[map_width]) new_map->_raw_map;

    for (row = 0; row < map_lenght; row++) {
        for (column = 0; column < map_width; column++) {
            map_matrix[row][column] = fgetwc(map_file);
        }
    }
    new_map->is_initialized = true;
}

wchar_t *map_get_wchar_from_coords(const Map *const map, uint8_t y, uint8_t x) {
    assert(map->is_initialized == true);

    wchar_t (*map_matrix)[map->_width] = (wchar_t(*)[map->_width]) map->_raw_map;
    return &map_matrix[y][x];
}

struct Position map_search_for_wchar(const Map *const map, wchar_t wch) {
    assert(map->is_initialized == true);

    struct Position wch_position;

    for (uint8_t row = 0; row < map->_length; row++) {
        for (uint8_t column = 0; column < map->_width; column++) {
            if (map_get_wchar_from_coords(map, row, column) == wch ) {
                wch_position.y = row;
                wch_position.x = column;
            }
        }
    }

    return wch_position;
}