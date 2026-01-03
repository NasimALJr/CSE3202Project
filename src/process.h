/* process.h - Process management header */
#ifndef PROCESS_H
#define PROCESS_H

#include "types.h"

typedef enum { READY, CURRENT, TERMINATED } process_state_t;

typedef struct process {
    uint32_t pid;
    process_state_t state;
    uint32_t esp;
    uint32_t eip;
    void* stack;
} process_t;

#define MAX_PROCESSES 10

extern process_t processes[MAX_PROCESSES];
extern int current_pid;

int create_process(void (*entry)());
void terminate_process(int pid);
void switch_to_process(int pid);
void init_processes();

#endif