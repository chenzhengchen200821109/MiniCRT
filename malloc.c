#include "minicrt.h"

typedef struct _heap_header
{
    enum {
        HEAP_BLOCK_FREE = 0xABABABAB, // magic number of free block
        HEAP_BLOCK_USED = 0xCDCDCDCD  // magic number of used block
    } type;                           // block type FREE/USED
    unsigned int size;                // block size including header
    struct _heap_header* next;
    struct _heap_header* prev;
} heap_header;

#define ADDR_ADD(addr, offset) ((char *)(addr) + (offset))
#define HEADER_SIZE (sizeof(heap_header))

static heap_header* list_head;

void minicrt_free(void* ptr)
{
    heap_header* header = (heap_header *)ADDR_ADD(ptr, -HEADER_SIZE);
    if (header->type != HEAP_BLOCK_USED)
        return;

    header->type = HEAP_BLOCK_FREE;
    if (header->prev && header->prev->type == HEAP_BLOCK_FREE) {
        // merge
        header->prev->next = header->next;
        if (header->next)
            header->next->prev = header->prev;
        header->prev->size += header->size;

        header = header->prev;
    }

    if (header->next && header->next->type == HEAP_BLOCK_FREE) {
        // merge
        header->size += header->next->size;
        header->next = header->next->next;
    }
}

void* minicrt_malloc(unsigned int size)
{
    heap_header* header;

    if (size == 0)
        return (void *)0;

    header = list_head;
    while (header != 0) {
        if (header->type == HEAP_BLOCK_USED) {
            header = header->next;
            continue;
        }

        if (header->size > size + HEADER_SIZE && header->size <= size + 2 * HEADER_SIZE) {
            header->type = HEAP_BLOCK_USED;
            return ADDR_ADD(header, HEADER_SIZE);
        } else if (header->size > size + 2 * HEADER_SIZE) {
            // split
            heap_header* next = (heap_header *)ADDR_ADD(header, size + HEADER_SIZE);
            next->prev = header;
            next->next = header->next;
            next->type = HEAP_BLOCK_FREE;
            next->size = header->size - (size - HEADER_SIZE);
            header->next = next;
            header->size = size + HEADER_SIZE;
            header->type = HEAP_BLOCK_USED;
            return ADDR_ADD(header, HEADER_SIZE);
        }
        header = header->next;
    }

    return (void *)0;
}

// Linux brk system call
static int brk(void* end_data_segment)
{
    int ret = 0;
    // brk system call number: 45
    // in /usr/include/??? #define __NR_brk 45
    asm("movl $45, %%eax \n\t" "movl %1, %%ebx \n\t" "int $0x80 \n\t" "movl %%eax, %0 \n\t" : "=r"(ret) : "m"(end_data_segment));
}

int minicrt_heap_init()
{
    void* base = (void *)0;
    heap_header* header = (void *)0;
    // 32 MB heap size
    unsigned int heap_size = 1024 * 1024 * 32;

    base = (void *)brk(0);
    void* end = ADDR_ADD(base, heap_size);
    end = (void *)brk(end);
    if (!end)
        return 0;

    header = (heap_header *)base;
    header->size = heap_size;
    header->type = HEAP_BLOCK_FREE;
    header->next = (void *)0;
    header->prev = (void *)0;

    list_head = header;
    return 1;
}