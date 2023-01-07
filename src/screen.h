#ifndef MEMEDIT_SCREEN_H
#define MEMEDIT_SCREEN_H

#include "proc.h"
#include "mem.h"

void display_centered_text(int cols, int y, const char* text, unsigned len = 0xffffffff);
void display_process_list_screen(int cols, int lines, task_struct* processes, unsigned sel);
void display_table_header(int y, int x, const char* text, unsigned len);
void display_table_cell(int y, int x, const char* text, unsigned len, bool highlight);
void display_col_pid(int cols, int lines, int y, int x, int len, task_struct* processes, unsigned sel);
void display_col_mem(int cols, int lines, int y, int x, int len, task_struct* processes, unsigned sel);
void display_col_comm(int cols, int lines, int y, int x, int len, task_struct* processes, unsigned sel);
void display_col_cmdline(int cols, int lines, int y, int x, int len, task_struct* processes, unsigned sel);
void display_maps_list_screen(int cols, int lines, const std::vector<vm_maps_data>&, unsigned sel);
void display_col_addr(int cols, int lines, int y, int x, int len, const std::vector<vm_maps_data>&, unsigned sel);
void display_col_perm(int cols, int lines, int y, int x, int len, const std::vector<vm_maps_data>&, unsigned sel);
void display_col_offset(int cols, int lines, int y, int x, int len, const std::vector<vm_maps_data>&, unsigned sel);
void display_col_device(int cols, int lines, int y, int x, int len, const std::vector<vm_maps_data>&, unsigned sel);
void display_col_inode(int cols, int lines, int y, int x, int len, const std::vector<vm_maps_data>&, unsigned sel);
void display_col_file_path(int cols, int lines, int y, int x, int len, const std::vector<vm_maps_data>&, unsigned sel);
void display_mem_screen(int cols, int lines, const char* mem, unsigned long long l, unsigned long long r, unsigned long long curr, int items, bool mode, char input);

#endif //MEMEDIT_SCREEN_H
