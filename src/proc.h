#ifndef MEMEDIT_PROC_H
#define MEMEDIT_PROC_H

#include <csignal>

struct task_struct {
    pid_t pid;
    char comm[128];
    char cmdline[256];
    unsigned mem;
    task_struct* next;
};

task_struct* list_processes(task_struct* old = nullptr);
void free_process_list(task_struct*);
void read_process_comm(task_struct*);
void read_process_cmdline(task_struct*);
void read_process_mem(task_struct*);

#endif //MEMEDIT_PROC_H
