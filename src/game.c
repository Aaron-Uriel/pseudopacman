#include <ncurses.h>
#include "map.h"
#include "entity.h"

void draw_entities(WINDOW *const window, const Entity *entities[ENTITY_LIMIT]);

void main_game() {
    // Inicialización de entidades y objetos
    Map *const map = map_init();
    Entity *entities[ENTITY_LIMIT];
    for (enum EntityID id = 0; id < ENTITY_LIMIT; id++) {
        entities[id] = entity_init(map, id, L"██");
    }

    // Inicialización de las ventanas
    const struct Resolution game_window_resolution = map_get_size(map, KIND_REAL);
    WINDOW *const game_window_border = newwin(game_window_resolution.length+2, game_window_resolution.width+2, 0, 0);
    WINDOW *const game_window        = newwin(game_window_resolution.length, game_window_resolution.width, 1, 1);

    // Dibujamos el mapa (sabemos que nunca va a cambiar durante el juego)
    draw_window_borders(game_window_border);
    wrefresh(game_window_border);
    map_draw(map, game_window);
    wrefresh(game_window);

    do {
        draw_entities(game_window, /*(const Entity**)*/entities);

        wgetch(game_window);
    } while(true);

}

void draw_entities(WINDOW *const window, const Entity *entities[ENTITY_LIMIT]) {
    const  Entity   *current_entity;
    struct Position current_entity_pos;
    for (uint8_t i = 0; i < ENTITY_LIMIT; i++) {
        current_entity     = entities[i];
        current_entity_pos = entity_get_position(current_entity, KIND_REAL);
        mvwprintw(window, current_entity_pos.y, current_entity_pos.x, "%ls", current_entity->aspect);
    }

}