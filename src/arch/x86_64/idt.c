#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/isr.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void idt_set_entry(idt_entry_t *entry, void (*func)()) {
  entry->base_low = (uint16_t)(uint64_t)func;
  entry->base_mid = (uint16_t)((uint64_t)func >> 16);
  entry->base_high = (uint32_t)((uint64_t)func >> 32);
  entry->sel = 8;
  entry->flags = idt_a_present | idt_a_ring_0 | idt_a_type_interrupt;
}

int init_idt() {
  __asm__ volatile("cli");

  idtp.limit = sizeof(idt) - 1;
  idtp.base = (uint64_t)&idt;

  __asm__ volatile("lidt %0" : : "m"(idtp));

  return 0;
}
