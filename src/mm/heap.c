#include <stddef.h>
#include <stdint.h>
#include <halt.h>
#include <printf.h>
#include <mm/heap.h>
#include <mm/pmm.h>
#include <mm/vmm.h>

struct allocated {
  uint64_t magic_number;
  size_t pages;
} __attribute__((__packed__));

// Qword-like allocator :meme:
#define ALIGN_UP(__number) (((__number) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define ALIGN_DOWN(__number) ((__number) & ~(PAGE_SIZE - 1))

#define MAGIC_NUMBER 0x4730573902840265

int init_heap() {
  // nothing
  return 0;
}

void *kmalloc(size_t size) {
  size_t pages = ALIGN_UP(size) / PAGE_SIZE;
  void *ret = pmalloc(pages + 1);
  if (!ret) {
    return NULL;
  }
  ret += PHYS_MEM_OFFSET;
  struct allocated *alloc = (struct allocated *) ret;
  alloc->magic_number = MAGIC_NUMBER;
  alloc->pages = pages;
  return ret += PAGE_SIZE;
}

void kfree(void *mem) {
  struct allocated *alloc = (struct allocated *) ((uintptr_t) mem - PAGE_SIZE);
  if (alloc->magic_number != MAGIC_NUMBER) {
    return;
  }
  size_t pages = alloc->pages + 1;
  mem -= PHYS_MEM_OFFSET;
  pfree(mem - PAGE_SIZE, pages);
}
