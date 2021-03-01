#include <hw.h>
#include <kernel/irq.h>
#include <kernel/pit.h>
#include <stdint.h>

// I usally remove all comments that are not my own because I don't like
// unnecessary comments that I don't use but I can't get rid of this one because
// it is too funny.
/* Handles the timer. In this case, it's very simple: We
 *  increment the 'timer_ticks' variable every time the
 *  timer fires. By default, the timer fires 18.222 times
 *  per second. Why 18.222Hz? Some engineer at IBM must've
 *  been smoking something funky */
// End funny

void sleep(uint64_t milliseconds) {
  uint64_t eticks;

  eticks = timer_ticks + milliseconds;
  while (timer_ticks < eticks) {
    __asm__ __volatile__("sti//hlt//cli");
  }
}

void timer_handler(register_t *r) {
  timer_ticks++;

  // if (timer_ticks % 1000 == 0)
  // {
  //    printf("One second has passed\r\n");
  // }
}

void timer_phase(int hz) {
  int divisor = 1193180 / hz; /* Calculate our divisor */
  outb(0x43, 0x36);           /* Set our command byte 0x36 */
  outb(0x40, divisor & 0xFF); /* Set low byte of divisor */
  outb(0x40, divisor >> 8);   /* Set high byte of divisor */
}

int init_timer() {
  timer_phase(1000);
  timer_ticks = 0;
  irq_install_handler(0, timer_handler);
  return 0;
}