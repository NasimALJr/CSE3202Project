/* process.c - Process management implementation */
#include "process.h"
#include "memory.h"

process_t processes[MAX_PROCESSES];
int current_pid = -1;
static int next_pid = 0;

void init_processes() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].pid = -1;
        processes[i].state = TERMINATED;
    }
}

int create_process(void (*entry)()) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].state == TERMINATED) {
            processes[i].pid = next_pid++;
            processes[i].state = READY;
            processes[i].eip = (uint32_t)entry;
            processes[i].stack = alloc_stack();
            if (!processes[i].stack) return -1;
            processes[i].esp = (uint32_t)processes[i].stack + STACK_SIZE;
            return processes[i].pid;
        }
    }
    return -1;
}

void terminate_process(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == pid) {
            processes[i].state = TERMINATED;
            free_stack(processes[i].stack);
            if (current_pid == pid) {
                current_pid = -1;
            }
            return;
        }
    }
}

void switch_to_process(int pid) {
    process_t* proc = NULL;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == pid) {
            proc = &processes[i];
            break;
        }
    }
    if (!proc || proc->state != READY) return;
    proc->state = CURRENT;
    current_pid = pid;
    // switch stack
    __asm__ volatile ("mov %0, %%esp" : : "r"(proc->esp));
    // call the function
    ((void (*)())proc->eip)();
    // after return, terminate
    terminate_process(pid);
}