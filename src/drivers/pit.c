#include <acpi/acpi.h>
#include <asm.h>
#include <drivers/apic.h>
#include <drivers/pit.h>
#include <stdint.h>
#include <sys/irq.h>

static volatile uint64_t timer_ticks = 0;

void pit_phase(int hz) {
  int divisor = 1193180 / hz;
  outb(0x43, 0x36);
  outb(0x40, divisor & 0xFF);
  outb(0x40, divisor >> 8);
}

void pit_handler(uint64_t rsp) {
  (void)rsp;
  timer_ticks++;
}

void sleep(uint64_t ticks) {
  uint64_t rest_ticks = timer_ticks + ticks;
  while (timer_ticks < rest_ticks)
    ;
}

uint32_t pit_read_count() {
  outb(0x43, 0);
  uint32_t counter = inb(0x40);
  counter |= inb(0x40) << 8;

  return counter;
}

int init_pit() {
  pit_phase(1000); // Phase to vibrate once every millisecond;
  irq_install_handler(0, pit_handler);
  ioapic_redirect_irq(madt_lapics[0]->apic_id, 2, 32, 1);
  return 0;
}
