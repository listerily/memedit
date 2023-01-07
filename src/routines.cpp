#include <curses.h>
#include <cstdlib>
#include "routines.h"
#include "proc.h"
#include "util.h"
#include "screen.h"
#include "mem.h"

void select_maps_loop(pid_t pid) {
    unsigned sel = 0;
    while (true) {
        clear();
        auto maps_data = get_vm_maps_data(pid);
        if (maps_data.empty())
            break;
        if (sel >= maps_data.size())
            sel = maps_data.size() - 1;
        display_maps_list_screen(COLS, LINES, maps_data, sel);
        wtimeout(stdscr, 1000);
        int ch = wgetch(stdscr);

        if (ch == 'q' || ch == 'Q' || ch == KEY_LEFT) {
            break;
        } else if (ch == KEY_UP) {
            if (sel > 0)
                --sel;
        } else if (ch == KEY_DOWN) {
            if (sel + 1 < maps_data.size())
                ++sel;
        } else if (ch == KEY_ENTER || ch == KEY_RIGHT || ch == '\n') {
            edit_memory_loop(pid, maps_data[sel].begin, maps_data[sel].end);
        }
    }
}

void select_process_loop() {
    task_struct *processes = nullptr;
    pid_t current_pid = 1;
    while (true) {
        clear();
        processes = list_processes(processes);
        unsigned ind = get_nearest_process_index(processes, current_pid);
        display_process_list_screen(COLS, LINES, processes, ind);
        refresh();
        wtimeout(stdscr, 1000);
        int ch = wgetch(stdscr);

        if (ch == 'q' || ch == 'Q' || ch == KEY_LEFT) {
            break;
        } else if (ch == KEY_UP) {
            if (ind > 0)
                current_pid = get_pid_from_index(processes, ind - 1);
        } else if (ch == KEY_DOWN) {
            if (ind + 1 < get_processes_num(processes))
                current_pid = get_pid_from_index(processes, ind + 1);
        } else if (ch == KEY_ENTER || ch == KEY_RIGHT || ch == '\n') {
            select_maps_loop(current_pid);
        }
    }
}

void edit_memory_loop(pid_t pid, unsigned long long begin, unsigned long long end) {
    unsigned long long curr = begin;
    unsigned long long top = begin;
    bool mode = false;
    char input = 0;
    int input_times = 0;
    while (true) {
        int items = (COLS - 16) / 4;
        if (items < 4) items = 4;
        unsigned long long bottom = top + items * (LINES - 5);
        unsigned long long rbd = end > bottom ? bottom : end;
        if (rbd - top > 0) {
            char *mem = new char[rbd - top];
            read_mem(pid, mem, (void *) top, rbd - top);
            clear();
            display_mem_screen(COLS, LINES, mem, top, rbd, curr, items, mode, input);
            refresh();
            delete[] mem;
        }
        wtimeout(stdscr, 1000);
        int ch = wgetch(stdscr);

        if (mode) {
            if (ch == 'q' || ch == 'Q') {
                mode = false;
            } else if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')) {
                input <<= 4;
                ++input_times;
                if (ch <= '9') {
                    input |= (ch - '0');
                } else if (ch >= 'a') {
                    input |= (ch - 'a' + 10);
                } else {
                    input |= (ch - 'A' + 10);
                }
                if (input_times == 2) {
                    mode = false;
                    write_mem_byte(pid, input, (void*)curr);
                    perror("err");
                }
            }
        } else {
            if (ch == 'q' || ch == 'Q') {
                break;
            } else if (ch == KEY_LEFT) {
                if (curr - 1 >= begin)
                    --curr;
                if (curr < top)
                    top -= items;
                if (top < begin)
                    top = begin;
            } else if (ch == KEY_RIGHT) {
                if (curr + 1 < end)
                    ++curr;
                if (curr >= bottom && top + items < end)
                    top += items;
            } else if (ch == KEY_UP) {
                if (curr - items >= begin)
                    curr -= items;
                if (curr < top)
                    top -= items;
                if (top < begin)
                    top = begin;
            } else if (ch == KEY_DOWN) {
                if (curr + items < end)
                    curr += items;
                if (curr >= bottom && top + items < end)
                    top += items;
            } else if (ch == KEY_ENTER || ch == '\n') {
                mode = true;
                input = 0;
                input_times = 0;
            }
        }
    }
}