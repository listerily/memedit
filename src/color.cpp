#include <curses.h>
#include "color.h"

void init_colors() {
    start_color();
    init_pair(COLOR_PAIR_REVERSE, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_PAIR_SELECTED, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_UNKNOWN_CHAR, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PAIR_UNKNOWN_CHAR_SELECTED, COLOR_RED, COLOR_WHITE);
    init_pair(COLOR_PAIR_EDITING, COLOR_GREEN, COLOR_WHITE);
    use_default_colors();
}
void set_color(int pair) {
    attron(COLOR_PAIR(pair));
}
void unset_color(int pair) {
    attroff(COLOR_PAIR(pair));
}
void set_default_color() {
    use_default_colors();
}