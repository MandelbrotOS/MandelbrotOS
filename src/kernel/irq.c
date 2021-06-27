#include <asm.h>
#include <kernel/idt.h>
#include <kernel/irq.h>

void (*irq_routines[16])() = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

extern void pit_handler();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void irq_remap(void) {
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);
}

int init_irq() {
  irq_remap();

  idt_set_gate(&idt[32 + 0], 0, irq0);
  idt_set_gate(&idt[32 + 1], 0, irq1);
  idt_set_gate(&idt[32 + 2], 0, irq2);
  idt_set_gate(&idt[32 + 3], 0, irq3);
  idt_set_gate(&idt[32 + 4], 0, irq4);
  idt_set_gate(&idt[32 + 5], 0, irq5);
  idt_set_gate(&idt[32 + 6], 0, irq6);
  idt_set_gate(&idt[32 + 7], 0, irq7);
  idt_set_gate(&idt[32 + 8], 0, irq8);
  idt_set_gate(&idt[32 + 9], 0, irq9);
  idt_set_gate(&idt[32 + 10], 0, irq10);
  idt_set_gate(&idt[32 + 11], 0, irq11);
  idt_set_gate(&idt[32 + 12], 0, irq12);
  idt_set_gate(&idt[32 + 13], 0, irq13);
  idt_set_gate(&idt[32 + 14], 0, irq14);
  idt_set_gate(&idt[32 + 15], 0, irq15);

  return 0;
}

void irq_install_handler(int irq, void (*handler)()) {
  irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) { irq_routines[irq] = 0; }

void c_irq_handler(int irqno) {
  void (*handler)() = irq_routines[irqno - 32];

  if (handler)
    handler();

  if (irqno >= 40)
    outb(0xA0, 0x20);

  outb(0x20, 0x20);
}
