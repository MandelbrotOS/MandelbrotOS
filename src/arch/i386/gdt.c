#include <hw.h>
#include <kernel/gdt.h>

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access,
                  uint8_t gran) {
  gdt[num].base_low = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high = (base >> 24) & 0xFF;

  gdt[num].limit_low = (limit & 0xFFFF);
  gdt[num].granularity = ((limit >> 16) & 0x0F);

  gdt[num].granularity |= (gran & 0xF0);
  gdt[num].access = access;
}

int init_gdt() {
  gp.limit = (sizeof(gdt_entry_t) * 5) - 1;
  gp.base = &gdt;

  gdt_set_gate(0, 0, 0, 0, 0); // Null segment

  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A,
               0xCF); // Code segment, 0x08 offset from GDT base

  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92,
               0xCF); // Data segment, 0x10 offset from GDT base

  gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment

  gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

  gdt_flush();

  return 0;
}