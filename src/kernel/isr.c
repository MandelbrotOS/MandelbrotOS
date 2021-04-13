#include <kernel/idt.h>
#include <kernel/isr.h>

// ASM handlers
extern void isr_0();
extern void isr_1();
extern void isr_2();
extern void isr_3();
extern void isr_4();
extern void isr_5();
extern void isr_6();
extern void isr_7();
extern void isr_8();
extern void isr_9();
extern void isr_10();
extern void isr_11();
extern void isr_12();
extern void isr_13();
extern void isr_14();
extern void isr_15();
extern void isr_16();
extern void isr_17();
extern void isr_18();
extern void isr_19();
extern void isr_20();
extern void isr_21();
extern void isr_22();
extern void isr_23();
extern void isr_24();
extern void isr_25();
extern void isr_26();
extern void isr_27();
extern void isr_28();
extern void isr_29();
extern void isr_30();
extern void isr_31();

// Exception messages
char *exception_messages[] = {
  "Division By Zero",
  "Debug",
  "Non Maskable Interrupt",
  "Breakpoint",
  "Into Detected Overflow",
  "Out of Bounds",
  "Invalid Opcode",
  "Device not Available",
  "Double Fault",
  "Coprocessor Segment Overrun",
  "Bad TSS",
  "Segment Not Present",
  "Stack Fault Exception",
  "General Protection Fault",
  "Page Fault",
  "[RESERVED]",
  "Floating Point Exception",
  "Alignment Check",
  "Machine Check",
  "SIMD Floating Point Exception",
  "Virtualization Exception",
  "[RESERVED]",
  "[RESERVED]",
  "[RESERVED]",
  "[RESERVED]",
  "[RESERVED]",
  "[RESERVED]",
  "[RESERVED]",
  "[RESERVED]",
  "[RESERVED]",
  "[RESERVED]",
  "[RESERVED]",
  "Security Exception",
};

// Initializes ISR's
int init_isr()
{
  idt_set_entry(&idt[0], 0, isr_0);
  idt_set_entry(&idt[1], 0, isr_1);
  idt_set_entry(&idt[2], 0, isr_2);
  idt_set_entry(&idt[3], 0, isr_3);
  idt_set_entry(&idt[4], 0, isr_4);
  idt_set_entry(&idt[5], 0, isr_5);
  idt_set_entry(&idt[6], 0, isr_6);
  idt_set_entry(&idt[7], 0, isr_7);
  idt_set_entry(&idt[8], 0, isr_8);
  idt_set_entry(&idt[9], 0, isr_9);
  idt_set_entry(&idt[10], 0, isr_10);
  idt_set_entry(&idt[11], 0, isr_11);
  idt_set_entry(&idt[12], 0, isr_12);
  idt_set_entry(&idt[13], 0, isr_13);
  idt_set_entry(&idt[14], 0, isr_14);
  idt_set_entry(&idt[15], 0, isr_15);
  idt_set_entry(&idt[16], 0, isr_16);
  idt_set_entry(&idt[17], 0, isr_17);
  idt_set_entry(&idt[18], 0, isr_18);
  idt_set_entry(&idt[19], 0, isr_19);
  idt_set_entry(&idt[20], 0, isr_20);
  idt_set_entry(&idt[21], 0, isr_21);
  idt_set_entry(&idt[22], 0, isr_22);
  idt_set_entry(&idt[23], 0, isr_23);
  idt_set_entry(&idt[24], 0, isr_24);
  idt_set_entry(&idt[25], 0, isr_25);
  idt_set_entry(&idt[26], 0, isr_26);
  idt_set_entry(&idt[27], 0, isr_27);
  idt_set_entry(&idt[28], 0, isr_28);
  idt_set_entry(&idt[29], 0, isr_29);
  idt_set_entry(&idt[30], 0, isr_30);
  idt_set_entry(&idt[31], 0, isr_31);

  return 0;
}

// Run whenever a fult is detected
void fault_handler(int ex_no)
{
  printf("\r\n%s: FAULT!\r\n", exception_messages[ex_no]);
  for (;;)
    ;
}
