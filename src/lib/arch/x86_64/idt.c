#include <kernel/idt.h>

idt_entry_t idt[256];
idt_ptr_t idtp;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
  idt[num].base_lo = base & 0xFFFF;
  idt[num].base_hi = (base >> 16) & 0xFFFF;
  idt[num].always0 = 0;
  idt[num].sel = sel;
  idt[num].flags = flags;
}

int init_idt() {
  idtp.limit = (sizeof(idt_entry_t) * 256) - 1;
  idtp.base = (uint32_t) &idt;

  memset(&idt, 0, sizeof(idt_entry_t) * 256);

  idt_load();

  return 0;
}