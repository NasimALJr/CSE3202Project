/* memory.c - Memory management implementation */
#include "memory.h"
#include "types.h"

#define ALIGN(size) (((size) + 3) & ~3)  // align to 4 bytes

extern uint32_t __kernel_end;

static uint8_t* heap_start = (uint8_t*)&__kernel_end;
static uint8_t* heap_end = heap_start + HEAP_SIZE;
static uint8_t* heap_current = heap_start;

typedef struct block {
    size_t size;
    int free;
    struct block* next;
} block_t;

static block_t* free_list = NULL;

static void init_heap() {
    free_list = (block_t*)heap_start;
    free_list->size = HEAP_SIZE - sizeof(block_t);
    free_list->free = 1;
    free_list->next = NULL;
    heap_current = heap_start + sizeof(block_t) + free_list->size;
}

void* kmalloc(size_t size) {
    size = ALIGN(size);
    if (!free_list) init_heap();
    block_t* curr = free_list;
    while (curr) {
        if (curr->free && curr->size >= size) {
            curr->free = 0;
            if (curr->size > size + sizeof(block_t)) {
                // split
                block_t* new_block = (block_t*)((uint8_t*)curr + sizeof(block_t) + size);
                new_block->size = curr->size - size - sizeof(block_t);
                new_block->free = 1;
                new_block->next = curr->next;
                curr->next = new_block;
                curr->size = size;
            }
            return (void*)((uint8_t*)curr + sizeof(block_t));
        }
        curr = curr->next;
    }
    return NULL;
}

void kfree(void* ptr) {
    if (!ptr) return;
    block_t* block = (block_t*)((uint8_t*)ptr - sizeof(block_t));
    block->free = 1;
    // merge with next
    if (block->next && block->next->free) {
        block->size += sizeof(block_t) + block->next->size;
        block->next = block->next->next;
    }
    // merge with prev, but for simplicity, not implemented
}

void* alloc_stack() {
    void* stack = kmalloc(STACK_SIZE);
    return stack;
}

void free_stack(void* stack) {
    kfree(stack);
}