#ifndef __PMM_H__
#define __PMM_H__

#include <boot/stivale2.h>
#include <stddef.h>

#define PAGE_SIZE 0x1000
#define PHYS_MEM_OFFSET 0xffff800000000000

void print_pmm_bitmap();
void *pmalloc(size_t pages);
void *pcalloc(size_t pages);
void free_pages(void *adr, size_t page_count);
int init_pmm(struct stivale2_struct_tag_memmap *memory_info);

#endif
