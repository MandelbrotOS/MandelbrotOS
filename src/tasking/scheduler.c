#include <drivers/apic.h>
#include <printf.h>
#include <sys/irq.h>
#include <tasking/scheduler.h>

void schedule() {
  lapic_eoi();
  printf("yikes we gtin schedy");
}

void scheduler_init() {
  printf("Init\r\n");
  irq_install_handler(16, schedule);
  printf("handeled\r\n");
  ioapic_redirect_irq(lapic_get_id(), 0, 16, 0);
  printf("redirected\r\n");
  while (1)
    asm volatile("sti");
}
