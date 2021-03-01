#include <kernel/alloc.h>
#include <string.h>

#define HEAP_SIZE 0x10000
#define HEAP_MAGIC 0x42
#define ALIGN_TYPE char
#define ALIGNMENT 16ul
#define ALIGN_INFO sizeof(ALIGN_TYPE) * 16

static uint32_t *heap;
static uint32_t index;

#define ALIGN(ptr)                                                             \
  if (ALIGNMENT > 1) {                                                         \
    uint32_t diff;                                                             \
    ptr = (void *)((uint32_t)ptr + ALIGN_INFO);                                \
    diff = (uint32_t)ptr & (ALIGNMENT - 1);                                    \
    if (diff != 0) {                                                           \
      diff = ALIGNMENT - diff;                                                 \
      ptr = (void *)((uint32_t)ptr + diff);                                    \
    }                                                                          \
    *((ALIGN_TYPE *)((uint32_t)ptr - ALIGN_INFO)) = diff + ALIGN_INFO;         \
  }

#define UNALIGN(ptr)                                                           \
  if (ALIGNMENT > 1) {                                                         \
    uint32_t diff = *((ALIGN_TYPE *)((uint32_t)ptr - ALIGN_INFO));             \
    if (diff < (ALIGNMENT + ALIGN_INFO)) {                                     \
      ptr = (void *)((uint32_t)ptr - diff);                                    \
    }                                                                          \
  }

int init_heap(uint32_t start) {
  heap = (uint32_t *)start;
  for (int i = 0; i < HEAP_SIZE; i++)
    heap[i] = 0;
  heap[0] = HEAP_MAGIC;
  index = 1;

  return 0;
}

int count() {
  int i = 0;
  uint32_t *iterator = heap + 1;
  do {
    iterator += iterator[0] + 1;
    i++;
  } while (iterator[0] != 0);
  return i;
}

void *malloc(uint32_t size) {
  if (size < 1)
    return NULL;

  size = size + ALIGNMENT + ALIGN_INFO;

  heap[index] = size;
  index += size + 1;

  void *p = (void *)(heap + index - size);
  ALIGN(p);

  return p;
}

void free(void *ptr) {
  (void)ptr;
  UNALIGN(ptr);
}

void *realloc(void *ptr, size_t len) {
  void *real;

  real = malloc(len);
  memset(real, 0, len);
  if (real)
    memcpy(real, ptr, len);
  free(ptr);
  return (real);
}

void *calloc(size_t n, size_t size) {
  size_t total = n * size;
  void *p = malloc(total);

  if (!p)
    return NULL;

  return memset(p, 0, total);
}