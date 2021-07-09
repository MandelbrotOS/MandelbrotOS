#ifndef __VMM_H__
#define __VMM_H__

#define KERNEL_MEM_OFFSET 0xffffffff80000000

void vmm_switch_map_to_kern();
int init_vmm();

#endif
