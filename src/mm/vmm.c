#include <mm/pmm.h>
#include <mm/vmm.h>
#include <stddef.h>
#include <stdint.h>

// The majority of this is reworked from limine's VMM code. Of course edited for
// the MandelbrotOS and other little fixes added

// Kernel map
uint64_t *kernel_map = NULL;

// Get next level of map
uint64_t *get_next_level(uint64_t *current_level, size_t entry) {
  if (!current_level[entry] & 1) {
    current_level[entry] = (uint64_t) pcalloc(1);
    current_level[entry] |= 0b11;
  }
  return (uint64_t *) (current_level[entry] & ~(0x1ff));
}

// Switch current pagemap
void vmm_switch_pagemap(uint64_t pagemap) {
  __asm__ volatile("mov %0, %%cr3" ::"r"(pagemap));
}

// Map virtual page to physical adress
void vmm_map_page(uint64_t *pagemap, uintptr_t physical_address,
                  uintptr_t virtual_address, uint64_t flags) {
  uint64_t *pml4, *pml3, *pml2, *pml1;
  size_t level4, level3, level2, level1;

  level4 = (size_t) (virtual_address & ((size_t)0x1ff << 39)) >> 39;
  level3 = (size_t) (virtual_address & ((size_t)0x1ff << 30)) >> 30;
  level2 = (size_t) (virtual_address & ((size_t)0x1ff << 21)) >> 21;
  level1 = (size_t)(virtual_address & ((size_t)0x1ff << 12)) >> 12;

  pml4 = pagemap;
  pml3 = get_next_level(pml4, level4);
  pml2 = get_next_level(pml3, level3);
  pml1 = get_next_level(pml2, level2);

  pml1[level1] = physical_address | flags;
}

// Unmap virtual adress
void vmm_unmap_page(uint64_t *pagemap, uintptr_t virtual_address) {
  uint64_t *pml4, *pml3, *pml2, *pml1;
  size_t level4, level3, level2, level1;

  level4 = (size_t) (virtual_address & ((uintptr_t)0x1ff << 39)) >> 39;
  level3 = (size_t) (virtual_address & ((uintptr_t)0x1ff << 30)) >> 30;
  level2 = (size_t) (virtual_address & ((uintptr_t)0x1ff << 21)) >> 21;
  level1 = (size_t) (virtual_address & ((uintptr_t)0x1ff << 12)) >> 12;

  pml4 = pagemap;
  pml3 = get_next_level(pml4, level4);
  pml2 = get_next_level(pml3, level3);
  pml1 = get_next_level(pml2, level2);

  pml1[level1] = 0;
}

// Initialize VMM
int init_vmm() {
  kernel_map = (uint64_t *) pcalloc(1);

  for (uintptr_t i = 0; i < 0x80000000; i += PAGE_SIZE) {
    vmm_map_page(kernel_map, i, i + KERNEL_MEM_OFFSET, 0b11);
  }

  for (uintptr_t i = 0; i < 0x100000000; i += PAGE_SIZE) {
    vmm_map_page(kernel_map, i, i + PHYS_MEM_OFFSET, 0b11);
  }

  // LAI workaround
  for (uintptr_t i = 0; i < 0x1000; i += PAGE_SIZE) {
    vmm_map_page(kernel_map, i, i, 0b11);
  }

  vmm_switch_pagemap((uint64_t) kernel_map);

  return 0;
}
