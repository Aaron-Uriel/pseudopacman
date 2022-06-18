#include <ncurses.h>
#include <locale.h>
#include <assert.h>
#include "utilities.h"

struct Resolution terminal_resolution;

enum Option { OPTION_START_GAME, OPTION_LIST_SCORES, OPTION_EDIT_SCORES, OPTION_EXIT, OPTION_LIMIT};
const char option_text[OPTION_LIMIT][30] = { "Nuevo juego.", "Listar puntajes", "Editar puntajes", "Salir"};

void set_color_pairs();
void main_game();

int main() {
    // Inicialización
    setlocale(LC_CTYPE, "");
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    getmaxyx(stdscr, terminal_resolution.length, terminal_resolution.width);
    set_color_pairs();
    assert(has_colors() == true);

    // Preparativos para mostrar el menú
    const struct Resolution menu_resolution = {
        .length = OPTION_LIMIT,
        .width  = 30
    };
    const struct Position menu_start_position = {
        .y = (terminal_resolution.length / 2) - (menu_resolution.length/2),
        .x = (terminal_resolution.width / 2)  - (menu_resolution.width/2)
    };
    WINDOW * const menu_window_border = newwin(menu_resolution.length+2, menu_resolution.width+2, menu_start_position.y-1, menu_start_position.x-1);
    WINDOW * const menu_window        = newwin(menu_resolution.length,   menu_resolution.width,   menu_start_position.y,   menu_start_position.x);
    keypad(menu_window, TRUE);

    draw_window_borders(menu_window_border);
    wrefresh(menu_window_border);

    //Bucle del menú
    enum Option selected_option = OPTION_START_GAME;
    bool is_enter_pressed = false;
    uint32_t input;
    while (true) {
        for (enum Option option = 0; option < OPTION_LIMIT; option++) {
            mvwprintw(menu_window, option, 0, " %d.- %-20s [ ]", option + 1, option_text[option]);
        }
        mvwaddch(menu_window, selected_option, 27, '*');

        input = wgetch(menu_window);
        mvwaddch(menu_window, selected_option, 27, ' ');
        switch (input) {
            case KEY_UP: case 'w': case 'W': selected_option--; break;
            case KEY_DOWN: case 's': case 'S': selected_option++; break;
            case '\n': case 'd': is_enter_pressed = true;
        }
        if (selected_option == OPTION_LIMIT) { selected_option = 0; }
        if (selected_option >  OPTION_LIMIT) { selected_option = OPTION_LIMIT - 1;}

        if (is_enter_pressed) {
            switch (selected_option) {
                case OPTION_START_GAME: main_game();
                case OPTION_EXIT: goto menu_loop_end;
            }
        }
    };
    menu_loop_end: ;
    return 0;
}

void set_color_pairs() {
    init_pair(ENTITY_PACMAN, COLOR_YELLOW, COLOR_BLACK);
    init_pair(ENTITY_BLINKY, COLOR_RED, COLOR_BLACK);
    init_pair(ENTITY_PINKY,  COLOR_WHITE, COLOR_BLACK);
    init_pair(ENTITY_INKY,   COLOR_CYAN, COLOR_BLACK);
    init_pair(ENTITY_CLYDE,  COLOR_MAGENTA, COLOR_BLACK);
}