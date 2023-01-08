#include <iostream>
#include <sys/uio.h>
#include <cstdio>
#include <cstring>
#include <curses.h>
#include "color.h"
#include "routines.h"
#include "mem.h"

using namespace std;

int mian() {
    size_t size = 0x55832f0b1000 - 0x55832f073000;
    char *buffer = new char[size];
    struct iovec local_iov{buffer, size};
    struct iovec remote_iov{(void *) 0x55832f073000, size};

    unsigned ret = process_vm_readv(28040, &local_iov, 1, &remote_iov, 1, 0);

    const unsigned int display_len = 32;
    for (int i = 0; i < size; i += display_len) {
        for (unsigned j = 0; j < display_len; ++j) {
            if (i + j < size) {
                printf("%02X", (unsigned int) (buffer[i + j] & 0xFF));
            } else {
                ::printf("  ");
            }
            if (j + 1 != display_len) {
                ::printf(" ");
            }
        }
        printf(" | ");
        for (unsigned j = 0; j < display_len; ++j) {
            if (i + j < size) {
                const char p = buffer[i + j];
                if (p >= 32 && p < 127)
                    printf("%c", buffer[i + j]);
                else
                    printf("\033[4m%c\033[m", buffer[i + j] ? '?' : '.');
            }
        }
        printf("\n");
    }
    return 0;
}


int main() {
    if (geteuid() != 0) {
        printf("Please run this program with root privileges.\n");
        return 0;
    }
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    clear();
    init_colors();
    select_process_loop();
    endwin();
    exit(0);
    return 0;
}
