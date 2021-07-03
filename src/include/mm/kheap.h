#ifndef __KHEAP_H__
#define __KHEAP_H__

#include <stddef.h>
#include <stdint.h>

typedef struct kheap_block {
  struct kheap_block *next;
  size_t size;
  size_t used;
  size_t block_size;
} kheap_block_t;

typedef struct kheap {
  kheap_block_t *fblock;
} kheap_t;

void *kmalloc(size_t size);
void kfree(void *ptr);
int init_heap();

#endif
