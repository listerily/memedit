#include <cstdio>
#include "util.h"

bool is_number(const char *str) {
    if (!*str)
        return false;
    while (*str) {
        if (*str >= '0' && *str <= '9')
            str++;
        else
            return false;
    }
    return true;
}

void mem_printf(char *dest, unsigned mem) {
    if (mem == 0)
        sprintf(dest, "0 B");
    else if (mem <= 9999)
        sprintf(dest, "%u K", mem);
    else if (mem / 1024 <= 9999)
        sprintf(dest, "%u M", mem / 1024);
    else if (mem / 1024 / 1024 <= 9999)
        sprintf(dest, "%u G", mem / 1024 / 1024);
    else if (mem / 1024 / 1024 / 1024 <= 9999)
        sprintf(dest, "%u T", mem / 1024 / 1024 / 1024);
    else
        sprintf(dest, "INF");
}

unsigned get_processes_num(task_struct *p) {
    unsigned n = 0;
    while (p) {
        ++n;
        p = p->next;
    }
    return n;
}

unsigned get_nearest_process_index(task_struct *p, pid_t pid) {
    unsigned n = 0;
    while (p && p->pid <= pid) {
        ++n;
        p = p->next;
    }
    return n - 1;
}

pid_t get_pid_from_index(task_struct *p, unsigned ind) {
    while (p) {
        if (ind == 0)
            return p->pid;
        --ind;
        p = p->next;
    }
    return 0;
}

void *find_mem(void *haystack, void *haystack_end, void *needle, void *needle_end) {
    while (haystack < haystack_end) {
        bool flag = true;
        for (auto j = (unsigned long long) needle; j < (unsigned long long) needle_end; ++j) {
            if (((unsigned long long) haystack) + (j - (unsigned long long) needle) >=
                (unsigned long long) haystack_end ||
                *((unsigned char *) (haystack) + (j - (unsigned long long) needle)) != *(unsigned char *)j) {
                flag = false;
                break;
            }
        }
        if (flag)
            return haystack;
        haystack = (void *) (1 + (unsigned long long) haystack);
    }
    return haystack_end;
}
void dump_mem(void* mem, unsigned long long size) {
    FILE* file = fopen("./dumpfile", "wb");
    fwrite(mem, 1, size, file);
    fclose(file);
}