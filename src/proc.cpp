#include <cstdlib>
#include <dirent.h>
#include <cstdio>
#include <cstring>
#include "proc.h"
#include "util.h"

task_struct* list_processes(task_struct* old) {
    if (old)
        free_process_list(old);
    task_struct* list = nullptr;
    DIR *dir = opendir("/proc/");
    struct dirent *ptr;
    while ((ptr = readdir(dir)) != nullptr) {
        if (ptr->d_type == DT_DIR) {
            if (is_number(ptr->d_name)) {
                auto* new_item = static_cast<task_struct *>(malloc(sizeof(task_struct)));
                new_item->pid = static_cast<pid_t>(strtol(ptr->d_name, nullptr, 10));
                new_item->next = nullptr;
                read_process_comm(new_item);
                read_process_cmdline(new_item);
                read_process_mem(new_item);
                if (new_item->mem == 0 && new_item->pid != 1) {
                    free(new_item);
                    continue;
                }
                task_struct* tmp_list = list;
                if (list) {
                    while (tmp_list->next) {
                        tmp_list = tmp_list->next;
                    }
                    tmp_list->next = new_item;
                } else list = new_item;
            }
        }
    }
    closedir(dir);
    return list;
}

void read_process_comm(task_struct* p) {
    char tmp[1024];
    sprintf(tmp, "/proc/%u/comm", p->pid);
    FILE *file = fopen(tmp, "r");
    if (file == nullptr) {
        strcpy(p->comm, "unknown");
        return;
    }
    fgets(p->comm, sizeof(p->comm) - 1, file);
    fclose(file);
    unsigned len = strlen(p->comm);
    if (len > 0 && p->comm[len - 1] == '\n')
        p->comm[len - 1] = 0;
}

void read_process_cmdline(task_struct* p) {
    char tmp[1024];
    sprintf(tmp, "/proc/%u/cmdline", p->pid);
    FILE* file = fopen(tmp, "r");
    if (file == nullptr) {
        p->cmdline[0] = 0;
        return;
    }
    fgets(p->cmdline, sizeof(p->cmdline) - 1, file);
    fclose(file);
    unsigned len = strlen(p->cmdline);
    if (len > 0 && p->cmdline[len - 1] == '\n')
        p->cmdline[len - 1] = 0;
}

void read_process_mem(task_struct* p) {
    char tmp[1024];
    sprintf(tmp, "/proc/%u/status", p->pid);
    FILE* file = fopen(tmp, "r");
    unsigned len = fread(tmp, 1024, 1, file);
    fclose(file);
    p->mem = 0;
    if (len != 0) {
        char* occ = strstr(tmp, "VmSize:");
        if (occ != nullptr) {
            p->mem = strtol(occ + 7, nullptr, 10);
        }
    }
}

void free_process_list(task_struct* old) {
    while (old) {
        task_struct* next = old->next;
        free(old);
        old = next;
    }
}