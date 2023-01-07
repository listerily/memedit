#ifndef MEMEDIT_ROUTINES_H
#define MEMEDIT_ROUTINES_H

#include <csignal>

void select_process_loop();
void select_maps_loop(pid_t);
void edit_memory_loop(pid_t, unsigned long long begin, unsigned long long end);

#endif //MEMEDIT_ROUTINES_H
