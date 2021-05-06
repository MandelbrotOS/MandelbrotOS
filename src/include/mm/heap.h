#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h>
#include <stdint.h>
#include <mm/vmm.h>

int init_heap();
void *kmalloc(size_t size);
void kfree(void *mem);

#endif
