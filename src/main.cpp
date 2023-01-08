#include <cstdio>
#include <curses.h>
#include <cstdlib>
#include "color.h"
#include "routines.h"

using namespace std;

int main() {
    if (geteuid() != 0) {
        printf("Please run this program with root privileges.\n");
        return 0;
    }
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    clear();
    init_colors();
    select_process_loop();
    endwin();
    exit(0);
    return 0;
}
