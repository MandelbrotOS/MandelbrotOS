#include <boot/stivale2.h>
#include <cpu_locals.h>
#include <mm/kheap.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <printf.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <tasking/smp.h>

void core_init() {
  load_gdt();
  load_idt();
  vmm_switch_map_to_kern();

  cpu_locals_t *local = kmalloc(sizeof(cpu_locals_t));
  set_locals(local);

  asm volatile("1:\n"
               "sti\n"
               "hlt\n"
               "jmp 1b\n");
}

int init_smp(struct stivale2_struct_tag_smp *smp_info) {
  uint8_t bsp_lapic_id = smp_info->bsp_lapic_id;

  for (size_t i = 0; i < smp_info->cpu_count; i++) {
    if (smp_info->smp_info[i].lapic_id == bsp_lapic_id)
      continue;

    smp_info->smp_info[i].target_stack =
        (uint64_t)pmalloc(32) + (PAGE_SIZE * 32) + PHYS_MEM_OFFSET;
    smp_info->smp_info[i].goto_address = (uint64_t)core_init;
  }

  cpu_locals_t *local = kmalloc(sizeof(cpu_locals_t));
  set_locals(local);

  return 0;
}
