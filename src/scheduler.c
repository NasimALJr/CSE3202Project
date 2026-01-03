/* scheduler.c - Scheduler implementation */
#include "scheduler.h"
#include "process.h"

void init_scheduler() {
    init_processes();
}

void schedule() {
    if (current_pid != -1) {
        processes[current_pid].state = READY;
    }
    int next = (current_pid + 1) % MAX_PROCESSES;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[next].state == READY) {
            switch_to_process(processes[next].pid);
            return;
        }
        next = (next + 1) % MAX_PROCESSES;
    }
    current_pid = -1;
}

void yield() {
    schedule();
}