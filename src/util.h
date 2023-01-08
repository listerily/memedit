#ifndef MEMEDIT_UTIL_H
#define MEMEDIT_UTIL_H

#include "proc.h"

bool is_number(const char* str);
void mem_printf(char*, unsigned);
unsigned get_processes_num(task_struct*);
unsigned get_nearest_process_index(task_struct*, pid_t);
pid_t get_pid_from_index(task_struct*, unsigned);
void* find_mem(void* haystack, void* haystack_end, void* needle, void* needle_end);

#endif //MEMEDIT_UTIL_H
