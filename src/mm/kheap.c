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

#define BLOCK_SIZE 4

typedef struct _KHEAPBLOCKBM {
  struct _KHEAPBLOCKBM *next;
  uint32_t size;
  uint32_t used;
  uint32_t bsize;
} KHEAPBLOCKBM;

typedef struct _KHEAPBM {
  KHEAPBLOCKBM *fblock;
} KHEAPBM;

KHEAPBM kernel_heap;

void k_heapBMInit(KHEAPBM *heap) { heap->fblock = 0; }

int k_heapBMAddBlock(KHEAPBM *heap, uint32_t addr, uint32_t size,
                     uint32_t bsize) {
  KHEAPBLOCKBM *b;
  uint32_t bcnt;
  uint32_t x;
  uint8_t *bm;

  b = (KHEAPBLOCKBM *)addr;
  b->size = size - sizeof(KHEAPBLOCKBM);
  b->bsize = bsize;

  b->next = heap->fblock;
  heap->fblock = b;

  bcnt = size / bsize;
  bm = (uint8_t *)&b[1];

  /* clear bitmap */
  for (x = 0; x < bcnt; ++x) {
    bm[x] = 0;
  }

  /* reserve room for bitmap */
  bcnt = (bcnt / bsize) * bsize < bcnt ? bcnt / bsize + 1 : bcnt / bsize;
  for (x = 0; x < bcnt; ++x) {
    bm[x] = 1;
  }

  b->used = bcnt;

  return 1;
}

static uint8_t k_heapBMGetNID(uint8_t a, uint8_t b) {
  uint8_t c;
  for (c = a + 1; c == b || c == 0; ++c)
    ;
  return c;
}

void *k_heapBMAlloc(KHEAPBM *heap, uint32_t size) {
  KHEAPBLOCKBM *b;
  uint8_t *bm;
  uint32_t bcnt;
  uint32_t x, y, z;
  uint32_t bneed;
  uint8_t nid;

  /* iterate blocks */
  for (b = heap->fblock; b; b = b->next) {
    /* check if block has enough room */
    if (b->size - b->used >= size) {
      bcnt = b->size / b->bsize;
      bneed = (size / b->bsize) * b->bsize < size ? size / b->bsize + 1
                                                  : size / b->bsize;
      bm = (uint8_t *)&b[1];
      for (x = 0; x < bcnt; ++x) {
        for (y = 0; bm[x + y] == 0 && y < bneed; ++y)
          ;
        /* we have enough, now allocate them */
        if (y == bneed) {
          /*
             must use id that does not match bm[x] or bm[y]

             two cases bm[x] + 1 == bm[y], then decide and check
          */
          nid = k_heapBMGetNID(bm[x - 1], bm[x + y]);

          for (z = 0; z < y; ++z) {
            bm[x + z] = nid;
          }

          /* count used blocks NOT bytes */
          b->used += y;
          return (void *)(x * b->bsize + (uintptr_t)&b[1]);
        }
        x += y;
      }
    }
  }

  return 0;
}

void k_heapBMFree(KHEAPBM *heap, void *ptr) {
  KHEAPBLOCKBM *b;
  uintptr_t ptroff;
  uint32_t bi, x;
  uint8_t *bm;
  uint8_t id;

  for (b = heap->fblock; b; b = b->next) {
    if ((uintptr_t)ptr > (uintptr_t)b &&
        (uintptr_t)ptr < (uintptr_t)b + b->size) {
      /* found block */
      ptroff = (uintptr_t)ptr - (uintptr_t)&b[1]; /* get offset to get block */
      /* block offset in BM */
      bi = ptroff / b->bsize;
      /* .. */
      bm = (uint8_t *)&b[1];
      /* clear allocation */
      id = bm[bi];
      for (x = bi; bm[x] == id && x < (b->size / b->bsize); ++x) {
        bm[x] = 0;
      }
      return;
    }
  }

  /* uhoh.. not good (need some kind of wrapper that supports run-time checking
   * of the heap) */
  return;
}

void *kmalloc(size_t size) { return k_heapBMAlloc(&kernel_heap, size); }

void kfree(void *ptr) { k_heapBMFree(&kernel_heap, ptr); }

int init_heap() {
  k_heapBMInit(&kernel_heap);
  k_heapBMAddBlock(&kernel_heap, (uintptr_t)pmalloc(3) + PHYS_MEM_OFFSET,
                   3 * PAGE_SIZE, BLOCK_SIZE);
  return 0;
}
