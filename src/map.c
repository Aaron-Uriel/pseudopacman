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
        column_end: ;
    }
    file_end: ;
    rewind(map_file);

    const uint8_t map_lenght = row, map_width = column;
    Map *const new_map = calloc(1, sizeof(*new_map) + sizeof(wchar_t[map_lenght][map_width]));
    new_map->_size.length = map_lenght;
    new_map->_size.width = map_width;

    wchar_t (*map_matrix)[map_width] = (wchar_t(*)[map_width]) new_map->_raw_map;

    wchar_t current_wch;
    for (row = 0; row < map_lenght; row++) {
        for (column = 0; column < map_width; column++) {
            current_wch = fgetwc(map_file);
            switch (current_wch) {
                case '\n':
                    continue;
                default:
                    map_matrix[row][column] = current_wch;
            }
        }
    }
    new_map->is_initialized = true;

    return new_map;
}

wchar_t *map_get_wchar_from_coords(const Map *const map, uint8_t y, uint8_t x) {
    assert(map->is_initialized == true);

    wchar_t (*map_matrix)[map->_size.width] = (wchar_t(*)[map->_size.width]) map->_raw_map;
    return &map_matrix[y][x];
}

struct Position map_search_for_wchar(Map *const map, const wchar_t wch, const enum ReplaceWch replace_wch) {
    assert(map->is_initialized == true);

    struct Position wch_position = { 0 };
    wchar_t *test_wch_ptr;
    for (uint8_t row = 0; row < map->_size.length; row++) {
        for (uint8_t column = 0; column < map->_size.width; column++) {
            test_wch_ptr = map_get_wchar_from_coords(map, row, column);

            if (*test_wch_ptr == wch ) {
                if (replace_wch == REPLACE_WCH_TRUE) { *test_wch_ptr = ' '; }
                wch_position.y = row;
                wch_position.x = column;
                goto loop_exit;
            }
        }
    }
    loop_exit: ;

    return wch_position;
}

struct Resolution map_get_size(const Map *const map) {
    assert(map->is_initialized == true);

    return map->_size;
}

void map_draw(const Map *const map, const WINDOW *const window) {
    assert(map->is_initialized == true);

    const struct Resolution map_size = map_get_size(map);
    struct Resolution window_size;
    getmaxyx(window, window_size.length, window_size.width);
    assert((map_size.length <= window_size.length) && (map_size.width <= window_size.width));

    wchar_t *current_wch;
    for (uint8_t row = 0; row < map_size.length; row++) {
        wmove(window, row, 0);
        for (uint8_t column = 0; column < map_size.width; column++) {
            current_wch = map_get_wchar_from_coords(map, row, column);
            wprintw(window, "%lc", *current_wch);
        }
    }
    
}