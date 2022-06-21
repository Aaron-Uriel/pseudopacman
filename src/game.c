#include <ncurses.h>
#include "map.h"
#include "entity.h"

void main_game() {
    // Inicialización de entidades y objetos
    Map *const map = map_init();
    Pacman *const pacman = pacman_init(map);
    Ghost *ghosts[ENTITY_LIMIT - 1];
    for (enum EntityID id = 1; id < ENTITY_LIMIT; id++) {
        ghosts[id] = ghost_init(map, id);
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
        wgetch(game_window);
    } while(true);

}