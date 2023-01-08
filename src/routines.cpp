#include <curses.h>
#include <cstdlib>
#include <cstring>
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

        if (ch == 'q' || ch == 'Q' || ch == KEY_LEFT || ch == KEY_EXIT) {
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

        if (ch == 'q' || ch == 'Q' || ch == KEY_LEFT || ch == KEY_EXIT) {
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

    unsigned char mode = 0;
    // for editing
    unsigned char edit_input = 0;
    int edit_input_idx = 0;
    // for searching
    unsigned char search_seq[64];
    int search_input_idx = 0;
    bool on_result = false;
    bool search_end = false;

    while (true) {
        int items = (COLS - 16) / 4;
        if (items < 4) items = 4;
        unsigned long long bottom = top + items * (LINES - 5);
        unsigned long long rbd = end > bottom ? bottom : end;
        if (rbd - top > 0) {
            char *mem = new char[rbd - top];
            read_mem(pid, mem, (void *) top, rbd - top);
            clear();
            if (mode == 0)
                display_mem_screen_viewing(COLS, LINES, mem, top, rbd, curr, items);
            else if (mode == 1)
                display_mem_screen_editing(COLS, LINES, mem, top, rbd, curr, items, edit_input);
            else if (mode == 2)
                display_mem_screen_searching(COLS, LINES, mem, top, rbd, curr, items, search_seq, search_input_idx, on_result, search_end);
            refresh();
            delete[] mem;
        }
        wtimeout(stdscr, 1000);
        int ch = wgetch(stdscr);

        if (mode == 1) {
            if (ch == 'q' || ch == 'Q' || ch == KEY_EXIT) {
                mode = 0;
            } else if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')) {
                edit_input <<= 4;
                ++edit_input_idx;
                if (ch <= '9') {
                    edit_input |= (ch - '0');
                } else if (ch >= 'a') {
                    edit_input |= (ch - 'a' + 10);
                } else {
                    edit_input |= (ch - 'A' + 10);
                }
                if (edit_input_idx == 2) {
                    mode = 0;
                    write_mem_byte(pid, reinterpret_cast<void*>(&edit_input), (void*)curr);
                }
            }
        } else if (mode == 0) {
            if (ch == 'q' || ch == 'Q' || ch == KEY_EXIT) {
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
                mode = 1;
                edit_input = 0;
                edit_input_idx = 0;
            } else if (ch == 's' || ch == 'S') {
                mode = 2;
                memset(search_seq, 0, sizeof(search_seq));
                search_input_idx = 0;
                on_result = false;
            } else if (ch == 'd' || ch == 'D') {
                char *mem = new char[end - begin];
                read_mem(pid, mem, (void *) begin, end - begin);
                dump_mem(mem, end - begin);
            }
        } else if (mode == 2) {
            if (ch == 'q' || ch == 'Q' || ch == KEY_EXIT) {
                mode = 0;
            } else if (!search_end && ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))) {
                if (search_input_idx >= 64)
                    continue;
                search_seq[search_input_idx / 2] <<= 4;
                if (ch <= '9') {
                    search_seq[search_input_idx / 2] |= (ch - '0');
                } else if (ch >= 'a') {
                    search_seq[search_input_idx / 2] |= (ch - 'a' + 10);
                } else {
                    search_seq[search_input_idx / 2] |= (ch - 'A' + 10);
                }
                ++search_input_idx;
            } else if (ch == KEY_ENTER || ch == '\n' || ch == KEY_DOWN || ch == KEY_RIGHT) {
                if (search_input_idx < 2 || curr == end || search_end)
                    continue;
                curr++;
                char *mem = new char[end - curr];
                read_mem(pid, mem, (void *) curr, end - curr);
                void* res = find_mem(mem, mem + end - curr, search_seq, search_seq + search_input_idx / 2);
                if (res < mem + end - curr) {
                    on_result = true;
                    curr += (unsigned long long) res - (unsigned long long) mem;
                    top += curr - top - (curr - top) % items;
                } else {
                    search_end = true;
                    curr = begin;
                    top = begin;
                }
                delete[] mem;
            }
        }
    }
}