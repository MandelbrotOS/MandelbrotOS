#include <asm.h>
#include <stdint.h>
#include <sys/irq.h>
#include <tasking/scheduler.h>

static volatile uint64_t timer_ticks = 0;

void pit_phase(int hz) {
  int divisor = 1193180 / hz;
  outb(0x43, 0x36);
  outb(0x40, divisor & 0xFF);
  outb(0x40, divisor >> 8);
}

void pit_handler(uint64_t rsp) {
  timer_ticks++;
  if (timer_ticks % 100 == 0)
    schedule(rsp);
}

void sleep(uint64_t ticks) {
  uint64_t rest_ticks = timer_ticks + ticks;
  while (timer_ticks < rest_ticks)
    ;
}

int init_pit() {
  pit_phase(1000); // Phase to vibrate once every millisecond;
  irq_install_handler(0, pit_handler);
  return 0;
}
