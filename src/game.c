#include <ncurses.h>
#include <assert.h>
#include <unistd.h>
#include "map.h"
#include "entity.h"

void draw_entities(WINDOW *const window, const Entity *entities[ENTITY_LIMIT]);
void handle_movements(Entity *const entities[ENTITY_LIMIT], const Map * const map);
void add_movement(struct Movement *movement);

void main_game() {
    // Inicialización de entidades y objetos
    Map *const map = map_init();
    Entity *entities[ENTITY_LIMIT];
    for (enum EntityID id = ENTITY_PACMAN; id < ENTITY_LIMIT; id++) {
        entities[id] = entity_init(map, id, L"██");
    }
    Entity *player = entities[0];

    // Inicialización de las ventanas
    const struct Resolution game_window_resolution = map_get_size(map);
    WINDOW *const game_window_border = newwin(game_window_resolution.length+2, game_window_resolution.width+2, 0, 0);
    WINDOW *const game_window        = newwin(game_window_resolution.length, game_window_resolution.width, 1, 1);
    keypad(game_window, true);
    nodelay(game_window, true);

    // Dibujamos el mapa (sabemos que nunca va a cambiar durante el juego)
    draw_window_borders(game_window_border);
    wrefresh(game_window_border);
    map_draw(map, game_window);
    wrefresh(game_window);

    int32_t input;
    do {
        draw_entities(game_window, entities);
        usleep(150000);

        input = wgetch(game_window);
        switch(input) {
            case KEY_UP:    case 'w': player->facing_direction = FACING_NORTH; break;
            case KEY_DOWN:  case 's': player->facing_direction = FACING_SOUTH; break;
            case KEY_RIGHT: case 'd': player->facing_direction = FACING_EAST; break;
            case KEY_LEFT:  case 'a': player->facing_direction = FACING_WEST; break;
            case ERR: ;
        }
        handle_movements(entities, map);
    } while(true);

}

void draw_entities(WINDOW *const window, const Entity *entities[ENTITY_LIMIT]) {
    const  Entity   *current_entity;
    struct Position current_pos, previous_pos;
    for (uint8_t i = 0; i < ENTITY_LIMIT; i++) {
        current_entity     = entities[i];
        current_pos  = entity_get_position(current_entity, POSITION_TYPE_CURRENT);
        previous_pos = entity_get_position(current_entity, POSITION_TYPE_PREVIOUS);

        mvwprintw(window, previous_pos.y, previous_pos.x, "  ");

        wattron  (window, COLOR_PAIR(current_entity->color));
        mvwprintw(window, current_pos.y, current_pos.x, "%ls", current_entity->aspect);
        wattroff (window, COLOR_PAIR(current_entity->color));
    }

}

void handle_movements(Entity *const entities[ENTITY_LIMIT], const Map * const map) {
    int8_t delta_y, delta_x;
    Entity *current_entity;
    for (uint8_t i = 0; i < ENTITY_LIMIT; i++) {
        delta_y = 0;
        delta_x = 0;
        current_entity = entities[i];
        switch (current_entity->facing_direction) {
            case FACING_NORTH: delta_y -= 1; break;
            case FACING_SOUTH: delta_y += 1; break;
            case FACING_EAST:  delta_x += 2; break;
            case FACING_WEST:  delta_x -= 2; break;
        }
        entity_new_relative_position(current_entity, map, delta_y, delta_x);
    }
}