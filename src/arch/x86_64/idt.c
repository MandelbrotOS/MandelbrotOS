#include <kernel/idt.h>
#include <string.h>
#include <stdbool.h>

static idt_entry_t idt[256];
static idt_ptr_t idtp;

void set_entry(idt_entry_t *entry, void (*func)()) {
  entry->base_low = (uint16_t) (uint64_t) func;
  entry->base_mid = (uint16_t) ((uint64_t) func >> 16);
  entry->base_high = (uint32_t) ((uint64_t) func >> 32);
  entry->sel = 8;
  entry->flags = idt_a_present | idt_a_ring_0 | idt_a_type_interrupt;
}

// How to add an interrupt:
// 0. Create an value for it in idt_vector
//    (values for exceptions are already there)
// 1. Add a line in interrupts.asm like this:
//    IDT_HANDLER double_fault
// 2. Create a handler for the interrupt like this:
//    IDT_HANDLER(double_fault, { /* ... code ... */ })
// 3. Add a set_entry call in init_idt() like this:
//    set_entry(&idt[idt_v_double_fault], double_fault_asm_idt_handler);

static bool testing_interrupts = false;
static bool test_succeded = false;

IDT_HANDLER(breakpoint, {
  if (testing_interrupts) {
    test_succeded = true;
    return;
  } else printf("breakpoint!\r\n");
})
STUB_IDT_HANDLER(double_fault)

int init_idt() {
  asm volatile("cli");

  set_entry(&idt[idt_v_breakpoint], breakpoint_asm_idt_handler);
  set_entry(&idt[idt_v_double_fault], double_fault_asm_idt_handler);

  idtp.limit = sizeof(idt) - 1;
  idtp.base = (uint64_t) &idt;

  __asm__ volatile("lidt %0; sti" :: "m"(idtp));
  testing_interrupts = true;
  test_succeded = false;
  asm volatile("int3");
  testing_interrupts = false;
  if (! test_succeded) {
    printf("failed to enable interrupts\r\n");
    asm volatile("cli; hlt");
  }

  return 0;
}
