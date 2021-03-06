#include <stdlib.h>
#include <assert.h>

#include <unistd.h>
#include <ncurses.h>

#include "world.h"

void draw_entities(WINDOW *const window, const Entity *const entities[ENTITY_LIMIT]);
void draw_info(WINDOW *const info_win, const Entity *const entities[ENTITY_LIMIT]);
void handle_movements(Entity *const entities[ENTITY_LIMIT], const Map * const map);

void main_game() {
    // Inicialización de entidades y objetos
    Map *const map = map_init();
    Entity *entities[ENTITY_LIMIT];
    for (enum EntityID id = ENTITY_PACMAN; id < ENTITY_LIMIT; id++) {
        entities[id] = entity_init(map, id, L'█');
    }
    Entity *player = entities[0];

    // Inicialización de las ventanas
    struct Resolution terminal_resolution;
    getmaxyx(stdscr, terminal_resolution.length, terminal_resolution.width);

    struct Resolution game_window_resolution = map_get_size(map);
    game_window_resolution.width *= CELL_AESTHETIC_WIDTH;
    WINDOW *const game_window_border = newwin(game_window_resolution.length+2, game_window_resolution.width+2, 0, 0);
    WINDOW *const game_window        = newwin(game_window_resolution.length, game_window_resolution.width, 1, 1);
    keypad(game_window, true);
    nodelay(game_window, true);

    struct Resolution info_window_border_resolution;
    info_window_border_resolution.length = terminal_resolution.length;
    info_window_border_resolution.width  = terminal_resolution.width - (game_window_resolution.width + 2);
    WINDOW *const info_window_border = newwin(info_window_border_resolution.length, info_window_border_resolution.width, 0, game_window_resolution.width+2);
    WINDOW *const info_window        = newwin(info_window_border_resolution.length - 2, info_window_border_resolution.width - 2, 1, game_window_resolution.width+2+1);
    
    // Dibujamos la interface del juego y el mapa
    draw_window_borders(game_window_border);
    draw_window_borders(info_window_border);
    wrefresh(game_window_border);
    wrefresh(info_window_border);
    map_draw(map, game_window);

    uint8_t frame_tick_counter = 1;
    int32_t input;
    do {
        if (frame_tick_counter == 1) {
            draw_entities(game_window, (const Entity **)entities);
            draw_info(info_window, (const Entity **)entities);
            //wrefresh(game_window);
            wrefresh(info_window);
        }

        input = wgetch(game_window);
        switch(input) {
            case KEY_UP:    case 'w': player->facing_direction = FACING_NORTH; break;
            case KEY_DOWN:  case 's': player->facing_direction = FACING_SOUTH; break;
            case KEY_RIGHT: case 'd': player->facing_direction = FACING_EAST; break;
            case KEY_LEFT:  case 'a': player->facing_direction = FACING_WEST; break;
        }

        if (frame_tick_counter == 25) {
            handle_movements(entities, map);
            frame_tick_counter = 0;
        }

        usleep(10000);
        frame_tick_counter++;
    } while(true);

}

void draw_entities(WINDOW *const window, const Entity *const entities[ENTITY_LIMIT]) {
    const  Entity   *current_entity;
    struct Position current_pos, previous_pos;
    for (uint8_t i = 0; i < ENTITY_LIMIT; i++) {
        current_entity = entities[i];
        current_pos  = entity_get_position(current_entity, POSITION_TYPE_CURRENT);
        previous_pos = entity_get_position(current_entity, POSITION_TYPE_PREVIOUS);

        // Los caracteres que representan el mapa ocupan dos espacios
        current_pos.x  *= CELL_AESTHETIC_WIDTH;
        previous_pos.x *= CELL_AESTHETIC_WIDTH;

        mvwprintw(window, previous_pos.y, previous_pos.x, "  ");

        wattron  (window, COLOR_PAIR(current_entity->color));
        mvwprintw(window, current_pos.y, current_pos.x, "%lc%lc", current_entity->aspect, current_entity->aspect);
        wattroff (window, COLOR_PAIR(current_entity->color));
    }

}

void draw_info(WINDOW *const info_win, const Entity *const entities[ENTITY_LIMIT]) {
    mvwprintw(info_win, 0, 0, "Posición de las entidades:");
    for (uint8_t i = 0 ; i < ENTITY_LIMIT; i++) {
        mvwprintw(info_win, i + 1, 0, "entities[%d]: {y = %d, x = %d}  ", i, entities[i]->_position.y, entities[i]->_position.x);
    }
}

void handle_movements(Entity *const entities[ENTITY_LIMIT], const Map * const map) {
    int8_t delta_y, delta_x;
    Entity *current_entity;
    for (uint8_t i = 0; i < ENTITY_LIMIT; i++) {
        delta_y = delta_x = 0;
        current_entity = entities[i];

        if (current_entity->id != ENTITY_PACMAN) {
            entity_perform(current_entity, map);
        }

        switch (current_entity->facing_direction) {
            case FACING_NORTH: delta_y -= 1; break;
            case FACING_SOUTH: delta_y += 1; break;
            case FACING_EAST:  delta_x += 1; break;
            case FACING_WEST:  delta_x -= 1; break;
            default: assert(false && "Default case should not be possible");
        }
        entity_set_new_relative_position(current_entity, map, delta_y, delta_x);

    }
}