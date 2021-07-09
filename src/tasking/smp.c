#include <boot/stivale2.h>
#include <cpu_locals.h>
#include <mm/kheap.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <stdint.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <tasking/smp.h>
#include <stddef.h>

void core_init() {
  load_gdt();
  load_idt();
  vmm_switch_map_to_kern();

  cpu_locals_t *local = kmalloc(sizeof(cpu_locals_t));
  set_locals(local);

  while (1)
    asm volatile("sti\n"
                 "hlt\n");
}

int init_smp(struct stivale2_struct_tag_smp *smp_info) {
  for (size_t i = 1; i < smp_info->cpu_count; i++) {
    smp_info->smp_info[i].target_stack = (uint64_t) pmalloc(32) + (PAGE_SIZE * 32);
    smp_info->smp_info[i].goto_address = (uint64_t) core_init;
  }
   
  cpu_locals_t *local = kmalloc(sizeof(cpu_locals_t));
  set_locals(local);

  return 0;
}
