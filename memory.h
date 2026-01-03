/* memory.h - Memory management header */
#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

#define HEAP_SIZE (64 * 1024 * 1024)  // 64MB heap
#define STACK_SIZE 4096  // 4KB per stack

// Heap allocation
void* kmalloc(size_t size);
void kfree(void* ptr);

// Stack allocation
void* alloc_stack();
void free_stack(void* stack);

#endif