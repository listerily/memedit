#ifndef MEMEDIT_COLOR_H
#define MEMEDIT_COLOR_H

#define COLOR_PAIR_REVERSE 1
#define COLOR_PAIR_SELECTED 2
#define COLOR_PAIR_UNKNOWN_CHAR 3
#define COLOR_PAIR_UNKNOWN_CHAR_SELECTED 4
#define COLOR_PAIR_EDITING 5

void init_colors();
void set_color(int pair);
void unset_color(int pair);
void set_default_color();

#endif //MEMEDIT_COLOR_H
