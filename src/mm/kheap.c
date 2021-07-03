#include <mm/kheap.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <printf.h>
#include <stddef.h>
#include <stdint.h>

// Since I am lazy I did not write this.
// Taken from: https://wiki.osdev.org/User:Pancakes/BitmapHeapImplementation
// Added 64 bit support, dynamic allocation, paging support stuff and frankly,
// sane names

#define BLOCK_SIZE 1

kheap_t kernel_heap;

void kheap_init_heap(kheap_t *heap) { heap->fblock = 0; }

void kheap_add_block(kheap_t *heap, uintptr_t addr, size_t size,
                     size_t block_size) {
  kheap_block_t *block = (kheap_block_t *)addr;
  block->size = size - sizeof(kheap_block_t);
  block->block_size = block_size;
  block->next = heap->fblock;

  heap->fblock = block;

  uint8_t *bm = (uint8_t *)&block[1];

  size_t bcnt = block->size / block->block_size;
  for (size_t x = 0; x < bcnt; x++)
    bm[x] = 0;

  bcnt = (bcnt / block_size) * block_size < bcnt ? bcnt / block_size + 1
                                                 : bcnt / block_size;
  for (size_t x = 0; x < bcnt; x++)
    bm[x] = 5;

  block->last_free_block = bcnt - 1;
  block->used = bcnt;
}

uint8_t kheap_get_nid(uint8_t a, uint8_t b) {
  uint8_t c;
  for (c = a + 1; c == b || c == 0; c++)
    ;
  return c;
}

void *kheap_alloc(kheap_t *heap, size_t size) {
  for (kheap_block_t *block = heap->fblock; block; block = block->next) {
    if (block->size - (block->used * block->block_size) >= size) {
      printf("whheeee\r\n");
      size_t bcnt = block->size / block->block_size;
      size_t bneed = (size / block->block_size) * block->block_size < size
                         ? size / block->block_size + 1
                         : size / block->block_size;
      uint8_t *bm = (uint8_t *)&block[1];

      for (size_t x = (block->last_free_block + 1 >= bcnt
                           ? 0
                           : block->last_free_block + 1);
           x < block->last_free_block; x++) {
        if (x >= bcnt)
          x = 0;
        
        printf("here");
        printf("%u", bm[x]);
        if (bm[x] == 0) {
          size_t y;
          printf("found\r\n");

          for (y = 0; bm[x + y] == 0 && y < bneed && (x + y) < bcnt; y++)
            ;

          if (y == bneed) {
            uint8_t nid = kheap_get_nid(bm[x - 1], bm[x + y]);

            for (size_t z = 0; z < y; z++)
              bm[x + z] = nid;

            block->last_free_block = (x + bneed) - 2;
            block->used += y;

            printf("ooga ting = %lu\r\n", (x * block->block_size +
            (uintptr_t)&block[1])); 

            return (void *)(x * block->block_size + (uintptr_t)&block[1]);
          }
        }
      }
    }
  }
  printf("mamm mia\r\n");
  return NULL;
}

void kheap_free(kheap_t *heap, void *ptr) {
  for (kheap_block_t *block = heap->fblock; block; block = block->next) {
    if ((uintptr_t)ptr > (uintptr_t)block &&
        (uintptr_t)ptr <
            (uintptr_t)block + sizeof(kheap_block_t) + block->size) {
      uintptr_t ptroff = (uintptr_t)ptr - (uintptr_t)&block[1];
      size_t bi = ptroff / block->block_size;
      uint8_t *bm = (uint8_t *)&block[1];
      uint8_t id = bm[bi];
      size_t x;

      for (x = bi; bm[x] == id && x < block->size / block->block_size; x++)
        bm[x] = 0;

      block->used -= x - bi;
      return;
    }
  }
}

void *kmalloc(size_t size) { return kheap_alloc(&kernel_heap, size); }

void kfree(void *ptr) { kheap_free(&kernel_heap, ptr); }

int init_heap() {
  kheap_init_heap(&kernel_heap);
  kheap_add_block(&kernel_heap, (uintptr_t)pmalloc(30), 30 * PAGE_SIZE,
                  BLOCK_SIZE);
  return 0;
}
