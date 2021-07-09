#include <boot/stivale2.h>
#include <drivers/apic.h>
#include <printf.h>
#include <sys/irq.h>
#include <tasking/scheduler.h>

#define STACK_SIZE 131072

void kidle() {
  printf("Reached kidle\r\n");
  while (1)
    ;
}

void schedule() { /* printf("yikes we gtin schedy"); */
}

void scheduler_init(struct stivale2_struct_tag_smp *smp) {
  printf("We have %lu cores \r\n", smp->cpu_count);
  while (1)
    asm volatile("sti");
}
