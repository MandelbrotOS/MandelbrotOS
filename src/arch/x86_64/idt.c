#include <kernel/idt.h>

idt_ptr_t idtp;
idt_entry_t idt[256];

void idt_set_gate(idt_entry_t *entry, int user_space, void (*func)()) {
  entry->base_low = (uint16_t)((uint64_t)(func));
  entry->base_mid = (uint16_t)((uint64_t)(func) >> 16);
  entry->base_high = (uint32_t)((uint64_t)(func) >> 32);
  entry->sel = 8;
  entry->flags = idt_a_present | (user_space ? idt_a_ring_3 : idt_a_ring_0) |
                 idt_a_type_interrupt;
}

int init_idt() {
  asm volatile("cli");

  idtp.limit = sizeof(idt) - 1;
  idtp.base = (uint64_t)&idt;

  asm volatile("lidt %0" : : "m"(idtp));

  return 0;
}
