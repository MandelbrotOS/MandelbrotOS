global idt_load
extern idtp
idt_load:
  lidt [idtp]
  ret