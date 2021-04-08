#include <boot/stivale2.h>
#include <mm/pmm.h>
#include <printf.h>
#include <stddef.h>
#include <string.h>

// Bit functions. Provided by AtieP
#define ALIGN_UP(__number) (((__number) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define ALIGN_DOWN(__number) ((__number) & ~(PAGE_SIZE - 1))

#define BIT_SET(__bit) (bitmap[(__bit) / 8] |= (1 << ((__bit) % 8)))
#define BIT_CLEAR(__bit) (bitmap[(__bit) / 8] &= ~(1 << ((__bit) % 8)))
#define BIT_TEST(__bit) ((bitmap[(__bit) / 8] >> ((__bit) % 8)) & 1)

size_t bitmap_size;
static uintptr_t highest_page = 0;
uint8_t *bitmap;

void free_page(void *adr) {
  uint32_t index = (uint32_t)(uintptr_t)adr / PAGE_SIZE;
  BIT_CLEAR(index);
}

void reserve_page(void *adr) {
  uint32_t index = (uint32_t)(uintptr_t)adr / PAGE_SIZE;
  BIT_SET(index);
}

void free_pages(void *adr, uint32_t page_count) {
  for (uint32_t i = 0; i < page_count; i++) {
    free_page((void *)(adr + (i * PAGE_SIZE)));
  }
}

void reserve_pages(void *adr, uint32_t page_count) {
  for (uint32_t i = 0; i < page_count; i++) {
    reserve_page((void *)(adr + (i * PAGE_SIZE)));
  }
}

void *pmalloc(uint32_t pages) {
  for (uint32_t i = 0; i < bitmap_size * 8; i++) {
    for (int j = 0; j < pages; j++) {
      if (BIT_TEST(i))
        break;
      else if (!BIT_TEST(i) && j == pages - 1) {
        reserve_pages((void *)(uintptr_t)(i * PAGE_SIZE), pages);
        return (void *)(uintptr_t)(i * PAGE_SIZE);
      }
    }
  }
  return NULL;
}

void *pcalloc(uint64_t pages) {
  uint64_t *p = (uint64_t *)pmalloc(pages);

  for (uint64_t i = 0; i < (pages * PAGE_SIZE); i++)
    p[i] = 0;
  return (void *)p;
}

// Init physical memory management
int init_pmm(struct stivale2_mmap_entry *memory_map, size_t memory_entries) {
  uintptr_t top;

  for (int i = 0; (size_t)i < memory_entries; i++) {
    struct stivale2_mmap_entry entry = memory_map[i];

    if (entry.type != STIVALE2_MMAP_USABLE &&
        entry.type != STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE &&
        entry.type != STIVALE2_MMAP_KERNEL_AND_MODULES)
      continue;

    top = entry.base + entry.length;

    if (top > highest_page)
      highest_page = top;
  }

  bitmap_size = ALIGN_UP(highest_page) / 8;

  for (int i = 0; (size_t)i < memory_entries; i++) {
    struct stivale2_mmap_entry entry = memory_map[i];

    if (entry.type != STIVALE2_MMAP_USABLE)
      continue;

    if (entry.length >= bitmap_size) {
      bitmap = (uint8_t *)entry.base;
      entry.base += bitmap_size;
      entry.length -= bitmap_size;
      break;
    }
  }

  for (int i = 0; (size_t)i < memory_entries; i++) {
    if (memory_map[i].type != STIVALE2_MMAP_USABLE)
      reserve_pages((void *)memory_map[i].base,
                    memory_map[i].length / PAGE_SIZE);
    else
      free_pages((void *)memory_map[i].base, memory_map[i].length / PAGE_SIZE);
  }

  return 0;
}
