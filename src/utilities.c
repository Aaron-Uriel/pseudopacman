#include "utilities.h"

void draw_window_borders(WINDOW *const window) {
    uint8_t height, width;
    getmaxyx(window, height, width);

    // Bordes
    mvwaddstr(window, 0, 0, "┏");
    mvwaddstr(window, height - 1, 0, "┗");
    mvwaddstr(window, 0, width - 1, "┓");
    mvwaddstr(window, height - 1, width - 1, "┛");

    // Lados
    uint8_t i;
    for (i = 1; i < (height - 1); i++) {
        mvwprintw(window, i, 0, u8"┃");
        mvwprintw(window, i, width - 1, u8"┃");
    }
    for (i = 1; i < (width - 1); i++) {
        mvwprintw(window, 0, i, u8"━");
        mvwprintw(window, height - 1, i, u8"━");
    }
}

// Regresa un valor u otro de los que se pasaron
double rand_one_or_the_other(const double x, const double y) {
    return (rand() % 2)? x: y;
}

char digit_to_char(const uint8_t digit) {
    return (digit + '0');
}