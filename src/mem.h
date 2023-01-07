#ifndef MEMEDIT_MEM_H
#define MEMEDIT_MEM_H

#include <csignal>
#include <vector>

struct vm_maps_data {
    unsigned long long begin, end;
    char addr_str[128];
    char perm[8];
    char offset_str[16];
    char device[8];
    char inode_str[64];
    char path_name[256];
};

std::vector<vm_maps_data> get_vm_maps_data(pid_t pid);
unsigned read_mem(pid_t, char*, void*, size_t);
unsigned write_mem_byte(pid_t, char, void*);

#endif //MEMEDIT_MEM_H
