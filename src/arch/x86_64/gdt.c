#include <kernel/gdt.h>
#include <stdint.h>

gdt_entry_t gdt[8];
gdt_pointer_t gdt_ptr;

void load_gdt(gdt_pointer_t gdt_ptr_in) {
  asm volatile("lgdt %0" : : "m"(gdt_ptr_in) : "memory");
  asm volatile("mov %%rsp, %%rax\n"
               "push $0x10\n"
               "push %%rax\n"
               "pushf\n"
               "push $0x8\n"
               "push $reload_segments\n"
               "iretq\n"
               "reload_segments:\n"
               "mov $0x10, %%ax\n"
               "mov %%ax, %%ds\n"
               "mov %%ax, %%es\n"
               "mov %%ax, %%ss\n"
               "mov %%ax, %%fs\n"
               "mov %%ax, %%gs\n"
               :
               :
               : "rax", "memory");
}

int init_gdt() {
  gdt_ptr.limit = sizeof(gdt) - 1;
  gdt_ptr.base = (uint64_t)gdt;

  gdt[0] = (gdt_entry_t){.low = 0x0,
                         .mid = 0x0,
                         .high = 0x0,
                         .limit = 0x0,
                         .access = 0x0,
                         .granularity = 0x0}; // NULL descriptor
  gdt[1] = (gdt_entry_t){.low = 0x0,
                         .mid = 0x0,
                         .high = 0x0,
                         .limit = 0x0,
                         .access = 0x9a,
                         .granularity = 0x20}; // Code descriptor
  gdt[2] = (gdt_entry_t){.low = 0x0,
                         .mid = 0x0,
                         .high = 0x0,
                         .limit = 0x0,
                         .access = 0x92,
                         .granularity = 0x0}; // Data descriptor

  load_gdt(gdt_ptr);

  return 0;
}
