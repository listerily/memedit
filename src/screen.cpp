#include "screen.h"
#include <cstring>
#include <curses.h>
#include "color.h"
#include "util.h"

void display_centered_text(int cols, int y, const char *text, unsigned len) {
    if (len == 0xffffffff) {
        len = strlen(text);
    }
    if (cols / 2 - len / 2 >= 0)
        mvaddstr(y, cols / 2 - len / 2, text);
}

void display_process_list_screen(int cols, int lines, task_struct *processes, unsigned sel) {
    display_centered_text(cols, 1, "Memory Editor v1.0");
    display_centered_text(cols, 2, "Quit (Q)   Select (Enter)   Up (Up)   Down (Down)");

    display_col_pid(cols, lines, 4, 0, 10, processes, sel);
    display_col_comm(cols, lines, 4, 10, 24, processes, sel);
    display_col_mem(cols, lines, 4, 34, 12, processes, sel);
    display_col_cmdline(cols, lines, 4, 46, cols - 47, processes, sel);
}

void display_table_header(int y, int x, const char *text, unsigned len) {
    unsigned str_len = strlen(text);
    char str[len + 1];
    for (unsigned i = 0; i < len; ++i)
        str[i] = (i >= str_len ? ' ' : text[i]);
    str[len] = 0;
    set_color(COLOR_PAIR_REVERSE);
    mvaddstr(y, x, str);
    unset_color(COLOR_PAIR_REVERSE);
}

void display_table_cell(int y, int x, const char *text, unsigned len, bool highlight) {
    unsigned str_len = strlen(text);
    char str[len + 1];
    for (unsigned i = 0; i < len; ++i)
        str[i] = (i >= str_len ? ' ' : text[i]);
    str[len] = 0;
    if (highlight)
        set_color(COLOR_PAIR_SELECTED);
    mvaddstr(y, x, str);
    if (highlight)
        unset_color(COLOR_PAIR_SELECTED);
}

void display_col_pid(int cols, int lines, int y, int x, int len, task_struct *processes, unsigned sel) {
    display_table_header(y, x, "PID", len);
    int size = lines - y - 2;
    int i = 0;
    task_struct *tmp = processes;
    char text[128];
    while (tmp && i != sel) {
        tmp = tmp->next;
        i++;
    }
    i = 0;
    while (tmp && i <= size) {
        sprintf(text, "%u", tmp->pid);
        display_table_cell(y + i + 1, x, text, len, i == 0);
        tmp = tmp->next;
        i++;
    }
}

void display_col_comm(int cols, int lines, int y, int x, int len, task_struct *processes, unsigned sel) {
    display_table_header(y, x, "Name", len);
    int size = lines - y - 2;
    int i = 0;
    task_struct *tmp = processes;
    while (tmp && i != sel) {
        tmp = tmp->next;
        i++;
    }
    i = 0;
    while (tmp && i <= size) {
        display_table_cell(y + 1 + i, x, tmp->comm, len, i == 0);
        tmp = tmp->next;
        i++;
    }
}

void display_col_cmdline(int cols, int lines, int y, int x, int len, task_struct *processes, unsigned sel) {
    display_table_header(y, x, "Command", len);
    int size = lines - y - 2;
    int i = 0;
    task_struct *tmp = processes;
    while (tmp && i != sel) {
        tmp = tmp->next;
        i++;
    }
    i = 0;
    while (tmp && i <= size) {
        display_table_cell(y + 1 + i, x, tmp->cmdline, len, i == 0);
        tmp = tmp->next;
        i++;
    }
}

void display_col_mem(int cols, int lines, int y, int x, int len, task_struct *processes, unsigned sel) {
    display_table_header(y, x, "Memory", len);
    int size = lines - y - 2;
    int i = 0;
    task_struct *tmp = processes;
    char text[128];
    while (tmp && i != sel) {
        tmp = tmp->next;
        i++;
    }
    i = 0;
    while (tmp && i <= size) {
        mem_printf(text, tmp->mem);
        display_table_cell(y + 1 + i, x, text, len, i == 0);
        tmp = tmp->next;
        i++;
    }
}

void display_maps_list_screen(int cols, int lines, const std::vector<vm_maps_data> &maps, unsigned sel) {
    display_centered_text(cols, 1, "Please select virtual memory segment.");
    display_centered_text(cols, 2, "Select (Enter)   Back (Q)   Up (Up)   Down (Down)");
    display_col_addr(cols, lines, 4, 0, 26, maps, sel);
    display_col_perm(cols, lines, 4, 26, 5, maps, sel);
    display_col_offset(cols, lines, 4, 31, 9, maps, sel);
    display_col_device(cols, lines, 4, 40, 8, maps, sel);
    display_col_inode(cols, lines, 4, 48, 12, maps, sel);
    display_col_file_path(cols, lines, 4, 60, cols - 60, maps, sel);
}

void display_col_addr(int cols, int lines, int y, int x, int len, const std::vector<vm_maps_data> &maps, unsigned sel) {
    display_table_header(y, x, "Address", len);
    int size = lines - y - 1;
    for (unsigned i = sel; i < sel + size && i < maps.size(); ++i) {
        display_table_cell((int) (y + 1 + i - sel), x, maps[i].addr_str, len, i == sel);
    }
}

void display_col_perm(int cols, int lines, int y, int x, int len, const std::vector<vm_maps_data> &maps, unsigned sel) {
    display_table_header(y, x, "Perm", len);
    int size = lines - y - 1;
    for (unsigned i = sel; i < sel + size && i < maps.size(); ++i) {
        display_table_cell((int) (y + 1 + i - sel), x, maps[i].perm, len, i == sel);
    }
}

void
display_col_offset(int cols, int lines, int y, int x, int len, const std::vector<vm_maps_data> &maps, unsigned sel) {
    display_table_header(y, x, "Offset", len);
    int size = lines - y - 1;
    for (unsigned i = sel; i < sel + size && i < maps.size(); ++i) {
        display_table_cell((int) (y + 1 + i - sel), x, maps[i].offset_str, len, i == sel);
    }
}

void
display_col_device(int cols, int lines, int y, int x, int len, const std::vector<vm_maps_data> &maps, unsigned sel) {
    display_table_header(y, x, "Dev", len);
    int size = lines - y - 1;
    for (unsigned i = sel; i < sel + size && i < maps.size(); ++i) {
        display_table_cell((int) (y + 1 + i - sel), x, maps[i].device, len, i == sel);
    }
}

void
display_col_inode(int cols, int lines, int y, int x, int len, const std::vector<vm_maps_data> &maps, unsigned sel) {
    display_table_header(y, x, "i-node", len);
    int size = lines - y - 1;
    for (unsigned i = sel; i < sel + size && i < maps.size(); ++i) {
        display_table_cell((int) (y + 1 + i - sel), x, maps[i].inode_str, len, i == sel);
    }
}

void
display_col_file_path(int cols, int lines, int y, int x, int len, const std::vector<vm_maps_data> &maps, unsigned sel) {
    display_table_header(y, x, "Path name", len);
    int size = lines - y - 1;
    for (unsigned i = sel; i < sel + size && i < maps.size(); ++i) {
        display_table_cell((int) (y + 1 + i - sel), x, maps[i].path_name, len, i == sel);
    }
}

void display_mem_screen_viewing(int cols, int lines, const char *mem, unsigned long long l, unsigned long long r,
                                unsigned long long curr, int items) {
    display_centered_text(cols, 1, "Viewing Memory");
    display_centered_text(cols, 2, "Select (Enter)   Back (Q)   Dump (D)   Search (S)");
    int address_x, hex_view_x, char_view_x;
    display_table_header(4, address_x = 0, "Address", 13);
    display_table_header(4, hex_view_x = 13, "Hex View", items * 3 + 1);
    display_table_header(4, char_view_x = 13 + items * 3 + 1, "Char View", cols - (13 + items * 3));

    char temp[128];
    for (unsigned long long addr = l, i = 0; addr < r; addr += items, ++i) {
        sprintf(temp, "%llx", addr);
        if (curr >= addr && curr < addr + items) set_color(COLOR_PAIR_REVERSE);
        mvaddstr(5 + (int) i, address_x, temp);
        if (curr >= addr && curr < addr + items) unset_color(COLOR_PAIR_REVERSE);
        for (int j = 0; j < items; ++j) {
            if (addr + j < r) {
                sprintf(temp, "%02X", (int) (mem[addr - l + j] & 0xFF));
                if (curr == addr + j) set_color(COLOR_PAIR_REVERSE);
                mvaddstr(5 + (int) i, hex_view_x + 3 * j, temp);
                if (curr == addr + j) unset_color(COLOR_PAIR_REVERSE);
                if (mem[addr - l + j] >= 32 && mem[addr - l + j] < 127) {
                    if (curr == addr + j) set_color( COLOR_PAIR_REVERSE);
                    mvaddch(5 + (int) i, char_view_x + j, mem[addr - l + j]);
                    if (curr == addr + j) unset_color( COLOR_PAIR_REVERSE);
                } else if (mem[addr - l + j] == 0) {
                    set_color(curr == addr + j ? COLOR_PAIR_UNKNOWN_CHAR_SELECTED : COLOR_PAIR_UNKNOWN_CHAR);
                    mvaddch(5 + (int) i, char_view_x + j, '.');
                    unset_color(curr == addr + j ? COLOR_PAIR_UNKNOWN_CHAR_SELECTED : COLOR_PAIR_UNKNOWN_CHAR);
                } else {
                    set_color(curr == addr + j ? COLOR_PAIR_UNKNOWN_CHAR_SELECTED : COLOR_PAIR_UNKNOWN_CHAR);
                    mvaddch(5 + (int) i, char_view_x + j, '?');
                    unset_color(curr == addr + j ? COLOR_PAIR_UNKNOWN_CHAR_SELECTED : COLOR_PAIR_UNKNOWN_CHAR);
                }
            }
        }
    }
}

void display_mem_screen_editing(int cols, int lines, const char *mem, unsigned long long l, unsigned long long r,
                                unsigned long long curr, int items, unsigned char input) {
    display_centered_text(cols, 1, "Editing Memory");
    display_centered_text(cols, 2, "Cancel (Q)");
    int address_x, hex_view_x, char_view_x;
    display_table_header(4, address_x = 0, "Address", 13);
    display_table_header(4, hex_view_x = 13, "Hex View", items * 3 + 1);
    display_table_header(4, char_view_x = 13 + items * 3 + 1, "Char View", cols - (13 + items * 3));

    char temp[128];
    for (unsigned long long addr = l, i = 0; addr < r; addr += items, ++i) {
        sprintf(temp, "%llx", addr);
        if (curr >= addr && curr < addr + items) set_color(COLOR_PAIR_REVERSE);
        mvaddstr(5 + (int) i, address_x, temp);
        if (curr >= addr && curr < addr + items) unset_color(COLOR_PAIR_REVERSE);
        for (int j = 0; j < items; ++j) {
            if (addr + j < r) {
                if (addr + j == curr) sprintf(temp, "%02X", (int) (input & 0xFF));
                else sprintf(temp, "%02X", (int) (mem[addr - l + j] & 0xFF));
                if (curr == addr + j) set_color(COLOR_PAIR_EDITING);
                mvaddstr(5 + (int) i, hex_view_x + 3 * j, temp);
                if (curr == addr + j) unset_color(COLOR_PAIR_EDITING);
                if (mem[addr - l + j] >= 32 && mem[addr - l + j] < 127) {
                    if (curr == addr + j) set_color( COLOR_PAIR_REVERSE);
                    mvaddch(5 + (int) i, char_view_x + j, mem[addr - l + j]);
                    if (curr == addr + j) unset_color( COLOR_PAIR_REVERSE);
                } else if (mem[addr - l + j] == 0) {
                    set_color(curr == addr + j ? COLOR_PAIR_UNKNOWN_CHAR_SELECTED : COLOR_PAIR_UNKNOWN_CHAR);
                    mvaddch(5 + (int) i, char_view_x + j, '.');
                    unset_color(curr == addr + j ? COLOR_PAIR_UNKNOWN_CHAR_SELECTED : COLOR_PAIR_UNKNOWN_CHAR);
                } else {
                    set_color(curr == addr + j ? COLOR_PAIR_UNKNOWN_CHAR_SELECTED : COLOR_PAIR_UNKNOWN_CHAR);
                    mvaddch(5 + (int) i, char_view_x + j, '?');
                    unset_color(curr == addr + j ? COLOR_PAIR_UNKNOWN_CHAR_SELECTED : COLOR_PAIR_UNKNOWN_CHAR);
                }
            }
        }
    }
}

void display_mem_screen_searching(int cols, int lines, const char *mem, unsigned long long l, unsigned long long r,
                                  unsigned long long curr, int items, unsigned char *input, unsigned input_len, bool on,
                                  bool search_end) {
    char temp[256];
    int temp_end = 0;
    temp_end += sprintf(temp, search_end ? "No More Matching Results" : "Searching Memory: ");
    for (unsigned i = 0; !search_end && i < input_len / 2; i++)
        temp_end += sprintf(temp + temp_end, "%02X ", (int)(input[i]));

    display_centered_text(cols, 1, temp);
    display_centered_text(cols, 2, "Cancel (Q)");

    int address_x, hex_view_x, char_view_x;
    display_table_header(4, address_x = 0, "Address", 13);
    display_table_header(4, hex_view_x = 13, "Hex View", items * 3 + 1);
    display_table_header(4, char_view_x = 13 + items * 3 + 1, "Char View", cols - (13 + items * 3));

    for (unsigned long long addr = l, i = 0; addr < r; addr += items, ++i) {
        sprintf(temp, "%llx", addr);
        if (curr >= addr && curr < addr + items) set_color(COLOR_PAIR_REVERSE);
        mvaddstr(5 + (int) i, address_x, temp);
        if (curr >= addr && curr < addr + items) unset_color(COLOR_PAIR_REVERSE);
        for (int j = 0; j < items; ++j) {
            if (addr + j < r) {
                sprintf(temp, "%02X", (int) (mem[addr - l + j] & 0xFF));
                if (on && curr <= addr + j && addr + j < curr + input_len / 2) set_color(COLOR_PAIR_REVERSE);
                mvaddstr(5 + (int) i, hex_view_x + 3 * j, temp);
                if (on && curr <= addr + j && addr + j < curr + input_len / 2) unset_color(COLOR_PAIR_REVERSE);
                if (mem[addr - l + j] >= 32 && mem[addr - l + j] < 127) {
                    if (curr == addr + j) set_color( COLOR_PAIR_REVERSE);
                    mvaddch(5 + (int) i, char_view_x + j, mem[addr - l + j]);
                    if (curr == addr + j) unset_color( COLOR_PAIR_REVERSE);
                } else if (mem[addr - l + j] == 0) {
                    set_color(curr == addr + j ? COLOR_PAIR_UNKNOWN_CHAR_SELECTED : COLOR_PAIR_UNKNOWN_CHAR);
                    mvaddch(5 + (int) i, char_view_x + j, '.');
                    unset_color(curr == addr + j ? COLOR_PAIR_UNKNOWN_CHAR_SELECTED : COLOR_PAIR_UNKNOWN_CHAR);
                } else {
                    set_color(curr == addr + j ? COLOR_PAIR_UNKNOWN_CHAR_SELECTED : COLOR_PAIR_UNKNOWN_CHAR);
                    mvaddch(5 + (int) i, char_view_x + j, '?');
                    unset_color(curr == addr + j ? COLOR_PAIR_UNKNOWN_CHAR_SELECTED : COLOR_PAIR_UNKNOWN_CHAR);
                }
            }
        }
    }
}
