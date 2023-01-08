#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/uio.h>
#include "mem.h"

std::vector<vm_maps_data> get_vm_maps_data(pid_t pid) {
    std::vector<vm_maps_data> result;
    char temp[1024];
    sprintf(temp, "/proc/%u/maps", pid);
    FILE* file = fopen(temp, "r");
    if (file == nullptr) {
        return result;
    }
    while (!feof(file)) {
        fgets(temp, 1024, file);
        vm_maps_data data{};
        sscanf(temp, "%s %s %s %s %s %s", data.addr_str, data.perm, data.offset_str, data.device, data.inode_str, data.path_name);
        if (strlen(data.addr_str) <= 25) {
            char* tmp = data.addr_str;
            data.begin = strtol(data.addr_str, &tmp, 16);
            data.end = strtol(tmp + 1, nullptr, 16);
            result.push_back(data);
        }
    }
    fclose(file);
    return result;
}

unsigned read_mem(pid_t pid, void* buffer, void* src, size_t size) {
    struct iovec local_iov{buffer, size};
    struct iovec remote_iov{src, size};
    return process_vm_readv(pid, &local_iov, 1, &remote_iov, 1, 0);
}

unsigned write_mem_byte(pid_t pid, void* c, void* src) {
    struct iovec local_iov{&c, 1};
    struct iovec remote_iov{src, 1};
    return process_vm_writev(pid, &local_iov, 1, &remote_iov, 1, 0);
}