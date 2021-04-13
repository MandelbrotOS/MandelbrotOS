#include <mm/heap.h>
#include <stddef.h>
#include <stdint.h>

// This code was taken from here and adapted so it looks more neat:
// https://wiki.osdev.org/User:Mrvn/LinkedListBucketHeapImplementation This is a
// Linked List Bucket Heap allocator

chunk_t *free_chunk[NUM_SIZES] = {NULL};
size_t mem_free = 0;
size_t mem_used = 0;
size_t mem_meta = 0;
chunk_t *first = NULL;
chunk_t *last = NULL;

// DLIST FUNCTIONS

int dlist_init(dlist_t *list) {
  list->next = list;
  list->prev = list;

  return 0;
}

void dlist_insert_after(dlist_t *d1, dlist_t *d2) {
  dlist_t *e1 = d1->next;
  dlist_t *e2 = d2->prev;

  d1->next = d2;
  d2->prev = d1;
  e2->next = e1;
  e1->prev = e2;
}

void dlist_insert_before(dlist_t *d1, dlist_t *d2) {
  dlist_t *e1 = d1->prev;
  dlist_t *e2 = d2->prev;

  e1->next = d2;
  d2->prev = e1;
  e2->next = d1;
  d1->prev = e2;
}

void dlist_remove(dlist_t *d) {
  d->prev->next = d->next;
  d->next->prev = d->prev;
  d->next = d;
  d->prev = d;
}

void dlist_push(dlist_t **d1p, dlist_t *d2) {
  if (*d1p != NULL) {
    dlist_insert_before(*d1p, d2);
  }
  *d1p = d2;
}

dlist_t *dlist_pop(dlist_t **dp) {
  dlist_t *d1 = *dp;
  dlist_t *d2 = d1->next;
  dlist_remove(d1);
  if (d1 == d2) {
    *dp = NULL;
  } else {
    *dp = d2;
  }
  return d1;
}

void dlist_remove_from(dlist_t **d1p, dlist_t *d2) {
  if (*d1p == d2) {
    dlist_pop(d1p);
  } else {
    dlist_remove(d2);
  }
}

// MEMORY FUNCTIONS

static void memory_chunk_init(chunk_t *chunc) {
  DLIST_INIT(chunc, all);
  chunc->used = 0;
  DLIST_INIT(chunc, free);
}

static size_t memory_chunk_size(const chunk_t *chunc) {
  char *end = (char *)(chunc->all.next);
  char *start = (char *)(&chunc->all);
  return (end - start) - HEADER_SIZE;
}

static int memory_chunk_slot(size_t size) {
  int n = -1;
  while (size > 0) {
    ++n;
    size /= 2;
  }
  return n;
}

// EXTRA FUNCTIONS FOR FREE

static void remove_free(chunk_t *chunk) {
  size_t len = memory_chunk_size(chunk);
  int n = memory_chunk_slot(len);
  DLIST_REMOVE_FROM(&free_chunk[n], chunk, free);
  mem_free -= len - HEADER_SIZE;
}

static void push_free(chunk_t *chunk) {
  size_t len = memory_chunk_size(chunk);
  int n = memory_chunk_slot(len);
  DLIST_PUSH(&free_chunk[n], chunk, free);
  mem_free += len - HEADER_SIZE;
}

// INIT, FREE AND MALLOC

// Initialize heap
int init_heap(void *mem, size_t size) {
  if (mem == NULL)
    return 1;

  char *mem_start = (char *)(((intptr_t)mem + ALIGN - 1) & (~(ALIGN - 1)));
  char *mem_end = (char *)(((intptr_t)mem + size) & (~(ALIGN - 1)));
  first = (chunk_t *)mem_start;
  chunk_t *second = first + 1;
  last = ((chunk_t *)mem_end) - 1;
  memory_chunk_init(first);
  memory_chunk_init(second);
  memory_chunk_init(last);
  dlist_insert_after(&first->all, &second->all);
  dlist_insert_after(&second->all, &last->all);
  first->used = 1;
  last->used = 1;

  size_t len = memory_chunk_size(second);
  int n = memory_chunk_slot(len);
  DLIST_PUSH(&free_chunk[n], second, free);
  mem_free = len - HEADER_SIZE;
  mem_meta = sizeof(chunk_t) * 2 + HEADER_SIZE;
  return 0;
}

// Allocate x amount of bytes
void *kmalloc(size_t size) {
  size = (size + ALIGN - 1) & (~(ALIGN - 1));

  if (size < MIN_SIZE)
    size = MIN_SIZE;

  int n = memory_chunk_slot(size - 1) + 1;

  if (n >= NUM_SIZES)
    return NULL;

  while (!free_chunk[n]) {
    ++n;
    if (n >= NUM_SIZES)
      return NULL;
  }

  chunk_t *chunk1 = DLIST_POP(&free_chunk[n], free);
  size_t size2 = memory_chunk_size(chunk1);
  size_t len = 0;

  if (size + sizeof(chunk_t) <= size2) {
    chunk_t *chunk2;
    chunk2 = (chunk_t *)(((char *)chunk1) + HEADER_SIZE + size);
    memory_chunk_init(chunk2);
    dlist_insert_after(&chunk1->all, &chunk2->all);
    len = memory_chunk_size(chunk2);
    int n = memory_chunk_slot(len);
    DLIST_PUSH(&free_chunk[n], chunk2, free);
    mem_meta += HEADER_SIZE;
    mem_free += len - HEADER_SIZE;
  }

  chunk1->used = 1;
  mem_free -= size2;
  mem_used += size2 - len - HEADER_SIZE;
  return chunk1->data;
}

// Free pointer
void kfree(void *mem) {
  chunk_t *chunk1 = (chunk_t *)((char *)mem - HEADER_SIZE);
  chunk_t *next = CONTAINER(chunk_t, all, chunk1->all.next);
  chunk_t *prev = CONTAINER(chunk_t, all, chunk1->all.prev);

  mem_used -= memory_chunk_size(chunk1);

  if (next->used == 0) {
    remove_free(next);
    dlist_remove(&next->all);
    mem_meta -= HEADER_SIZE;
    mem_free += HEADER_SIZE;
  }
  if (prev->used == 0) {
    remove_free(prev);
    dlist_remove(&chunk1->all);
    push_free(prev);
    mem_meta -= HEADER_SIZE;
    mem_free += HEADER_SIZE;
  } else {
    chunk1->used = 0;
    DLIST_INIT(chunk1, free);
    push_free(chunk1);
  }
}
