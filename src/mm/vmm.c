#include <mm/pmm.h>
#include <mm/vmm.h>
#include <stddef.h>
#include <stdint.h>

#define KERNEL_MAP_ADRESS 0x100000000
#define MEM_OFFSET 0xffff800000000000

// The majority of this is reworked from limine's VMM code. Of course edited for
// the MandelbrotOS and other little fixes added

uint64_t *kernel_map = NULL;

uint64_t *get_next_level(uint64_t *current_level, size_t entry) {
  uint64_t *ret;

  if (current_level[entry] & 0x1) {
    ret = (uint64_t *)(size_t)(current_level[entry] & ~((uint64_t)0xfff));
  } else {
    ret = pcalloc(1);
    if (ret == NULL)
      return NULL;
    current_level[entry] = (uint64_t)(size_t)ret | 0b111;
  }
  return ret;
}

void vmm_switch_pagemap(uint64_t pagemap) {
  __asm__ volatile("mov %0, %%cr3" ::"r"(pagemap));
}

void vmm_map_page(uint64_t *pagemap, uintptr_t physical_address,
                  uint64_t virtual_address, uintptr_t flags) {
  uintptr_t *pml4, *pml3, *pml2, *pml1;
  uintptr_t level4, level3, level2, level1;

  level4 = (virtual_address & ((uintptr_t)0x1ff << 39)) >> 39;
  level3 = (virtual_address & ((uintptr_t)0x1ff << 30)) >> 30;
  level2 = (virtual_address & ((uintptr_t)0x1ff << 21)) >> 21;
  level1 = (virtual_address & ((uintptr_t)0x1ff << 12)) >> 12;

  pml4 = (void *)pagemap;
  pml3 = get_next_level(pml4, level4);
  pml2 = get_next_level(pml3, level3);
  pml1 = get_next_level(pml2, level2);

  pml1[level1] = physical_address | flags;
}

void vmm_unmap_page(uint64_t *pagemap, uint64_t virtual_address) {
  uintptr_t *pml4, *pml3, *pml2, *pml1;
  uintptr_t level4, level3, level2, level1;

  level4 = (virtual_address & ((uintptr_t)0x1ff << 39)) >> 39;
  level3 = (virtual_address & ((uintptr_t)0x1ff << 30)) >> 30;
  level2 = (virtual_address & ((uintptr_t)0x1ff << 21)) >> 21;
  level1 = (virtual_address & ((uintptr_t)0x1ff << 12)) >> 12;

  pml4 = (void *)pagemap;
  pml3 = get_next_level(pml4, level4);
  pml2 = get_next_level(pml3, level3);
  pml1 = get_next_level(pml2, level2);

  pml1[level1] = 0;
}

int init_vmm() {
  kernel_map = pcalloc(1);

  if (kernel_map == NULL)
    return 1;
  for (uintptr_t i = 0; i < KERNEL_MAP_ADRESS; i += PAGE_SIZE) {
    vmm_map_page(kernel_map, i, i + MEM_OFFSET, 0x03);
  }

  vmm_switch_pagemap((uint64_t)&kernel_map);

  return 0;
}
