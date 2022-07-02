#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <assert.h>

#include <ncurses.h>

#include "utilities.h"
#include "world.h"

struct Resolution terminal_resolution;

void set_color_pairs();
void main_game();

int main() {
    // Inicialización
    setlocale(LC_CTYPE, "");
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    getmaxyx(stdscr, terminal_resolution.length, terminal_resolution.width);
    start_color();
    use_default_colors();
    set_color_pairs();
    assert(has_colors() == true);

    const uint8_t title_center = (terminal_resolution.width / 2) - (57 / 2);
    const uint8_t title_up_margin = terminal_resolution.length * 0.1;
    const wchar_t title_array[6][58] = {
        L"██████╗  █████╗  ██████╗    ███╗   ███╗ █████╗ ███╗   ██╗",
        L"██╔══██╗██╔══██╗██╔════╝    ████╗ ████║██╔══██╗████╗  ██║",
        L"██████╔╝███████║██║         ██╔████╔██║███████║██╔██╗ ██║",
        L"██╔═══╝ ██╔══██║██║         ██║╚██╔╝██║██╔══██║██║╚██╗██║",
        L"██║     ██║  ██║╚██████╗    ██║ ╚═╝ ██║██║  ██║██║ ╚████║",
        L"╚═╝     ╚═╝  ╚═╝ ╚═════╝    ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝"
    };
    for (uint8_t i = 0; i < 6; i++) {
        mvprintw(i + title_up_margin, title_center, "%S", title_array[i]);
    }
    refresh();

    const struct Resolution text_box_resolution = { .length = 1, .width  = 30 };
    const struct Position text_box_start_position = { 
        .y = (terminal_resolution.length / 2), 
        .x = (terminal_resolution.width / 2)  - (text_box_resolution.width/2 )
    };
    WINDOW * const text_box_window_border = newwin(text_box_resolution.length+2, text_box_resolution.width+2, text_box_start_position.y-1, text_box_start_position.x-1);
    WINDOW * const text_box_window        = newwin(text_box_resolution.length,   text_box_resolution.width,   text_box_start_position.y,   text_box_start_position.x);

    draw_window_borders(text_box_window_border);
    wrefresh(text_box_window_border);

    uint32_t input;                                                 
    do {
        wprintw(text_box_window, "Presione ENTER para iniciar.");
        input = wgetch(text_box_window);
    } while (input != '\n');

    main_game();

    return 0;
}

void set_color_pairs() {
    init_pair(COLOR_PACMAN, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLINKY, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PINKY,  COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_INKY,   COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_CLYDE,  COLOR_MAGENTA, COLOR_BLACK);
}