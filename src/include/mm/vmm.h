#ifndef __VMM_H__
#define __VMM_H__

// Memory offset
#define KERNEL_MEM_OFFSET 0xffffffff80000000
// Physical memory address offset
#define PHYS_MEM_OFFSET 0xffff800000000000

int init_vmm();
void vmm_map_page(uint64_t *pagemap, uintptr_t physical_address,
                  uintptr_t virtual_address, uint64_t flags);
void vmm_unmap_page(uint64_t *pagemap, uintptr_t virtual_address);

#endif
