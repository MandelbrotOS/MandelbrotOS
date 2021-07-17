#include <mm/kheap.h>
#include <mm/pmm.h>
#include <printf.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Since I am lazy I did not write this.
// Taken from: https://wiki.osdev.org/User:Pancakes/BitmapHeapImplementation
// Added dynamic allocation, paging support stuff, better names and some nice
// calling frontends

#define BLOCK_SIZE 4

kheap_t kernel_heap;

void kheap_init(kheap_t *heap) { heap->fblock = 0; }

int kheap_add_block(kheap_t *heap, void *addr, uint64_t size, uint64_t bsize) {
  kheap_block_t *block = (kheap_block_t *)(uint64_t)addr;
  block->size = size - sizeof(kheap_block_t);
  block->bsize = bsize;

  block->next = heap->fblock;
  heap->fblock = block;

  uint8_t *bm = (uint8_t *)&block[1];

  uint64_t bcnt = size / bsize;
  for (uint64_t x = 0; x < bcnt; x++)
    bm[x] = 0;

  bcnt = (bcnt / bsize) * bsize < bcnt ? bcnt / bsize + 1 : bcnt / bsize;
  for (uint64_t x = 0; x < bcnt; x++)
    bm[x] = 1;

  block->used = bcnt;

  return 1;
}

static uint8_t kheap_get_nid(uint8_t a, uint8_t b) {
  uint8_t c;
  for (c = a + 1; c == b || c == 0; ++c)
    ;
  return c;
}

void *kheap_alloc(kheap_t *heap, uint64_t size) {
  for (;;) {
    for (kheap_block_t *block = heap->fblock; block; block = block->next) {
      if (block->size - block->used >= size) {
        uint64_t bcnt = block->size / block->bsize;
        uint64_t bneed = (size / block->bsize) * block->bsize < size
                             ? size / block->bsize + 1
                             : size / block->bsize;
        uint8_t *bm = (uint8_t *)&block[1];

        for (uint64_t x = 0; x < bcnt; x++) {
          uint64_t y;

          for (y = 0; bm[x + y] == 0 && y < bneed; y++)
            ;

          if (y == bneed) {
            uint8_t nid = kheap_get_nid(bm[x - 1], bm[x + y]);

            for (uint64_t z = 0; z < y; z++)
              bm[x + z] = nid;

            block->used += y;

            return (void *)(x * block->bsize + (uintptr_t)&block[1]);
          }
          x += y;
        }
      }
    }
    kheap_add_block(&kernel_heap, (void *)(pmalloc(10) + PHYS_MEM_OFFSET),
                    10 * PAGE_SIZE, BLOCK_SIZE);
  }
}

void kheap_free(kheap_t *heap, void *ptr) {
  for (kheap_block_t *block = heap->fblock; block; block = block->next) {
    if ((uintptr_t)ptr > (uintptr_t)block &&
        (uintptr_t)ptr < (uintptr_t)block + block->size) {
      uint64_t bi = ((uintptr_t)ptr - (uintptr_t)&block[1]) / block->bsize;
      uint8_t *bm = (uint8_t *)&block[1];
      uint8_t id = bm[bi];

      for (uint64_t x = bi; bm[x] == id && x < (block->size / block->bsize);
           x++)
        bm[x] = 0;

      return;
    }
  }
}

void *kheap_realloc(kheap_t *heap, void *ptr, size_t size) {
  for (kheap_block_t *block = heap->fblock; block; block = block->next) {
    if ((uintptr_t)ptr > (uintptr_t)block &&
        (uintptr_t)ptr < (uintptr_t)block + block->size) {
      uint64_t bi = ((uintptr_t)ptr - (uintptr_t)&block[1]) / block->bsize;
      uint8_t *bm = (uint8_t *)&block[1];
      uint8_t id = bm[bi];

      size_t x;
      for (x = bi; bm[x] == id && x < (block->size / block->bsize); x++)
        ;
      x *= block->bsize;

      void *new_ptr = kheap_alloc(heap, size);
      memcpy(new_ptr, ptr, x);

      kheap_free(heap, ptr);

      return new_ptr;
    }
  }
  return kheap_alloc(heap, size);
}

void *kmalloc(size_t size) { return kheap_alloc(&kernel_heap, size); }

void kfree(void *ptr) { kheap_free(&kernel_heap, ptr); }

void *krealloc(void *ptr, size_t size) {
  return kheap_realloc(&kernel_heap, ptr, size);
}

void* kcalloc(size_t size) {
  void* ret = kmalloc(size);
  memset(ret, 0, size);
  return ret;
}

int init_heap() {
  kheap_init(&kernel_heap);
  kheap_add_block(&kernel_heap, (void *)(pmalloc(10) + PHYS_MEM_OFFSET),
                  10 * PAGE_SIZE, BLOCK_SIZE);
  return 0;
}
