#ifndef __KHEAP_H__
#define __KHEAP_H__

#include <stddef.h>
#include <stdint.h>

typedef struct kheap_block {
  struct kheap_block *next;
  uint64_t size;
  uint64_t used;
  uint64_t bsize;
} kheap_block_t;

typedef struct _kheap_t {
  kheap_block_t *fblock;
} kheap_t;

void *kmalloc(size_t size);
void kfree(void *ptr);
void *krealloc(void *ptr, size_t size);
int init_heap();

#endif
