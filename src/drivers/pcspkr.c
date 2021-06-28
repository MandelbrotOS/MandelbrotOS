#include <asm.h>
#include <drivers/pcspkr.h>
#include <drivers/pit.h>

void pit_phase_c2(uint32_t hz) {
  uint32_t divisor = 1193180 / hz;
  outb(0x43, 0xB6);
  outb(0x42, (uint8_t)divisor & 0xFF);
  outb(0x42, (uint8_t)(divisor >> 8) & 0xFF);
}

void pcspkr_tone_on(uint32_t hz) {
  pit_phase_c2(hz);
  uint8_t tmp = inb(0x61);
  if (tmp != (tmp | 3))
    outb(0x61, tmp | 3);
}

void pcspkr_tone_off() {
  uint8_t tmp = inb(0x61) & 0xFC;
  outb(0x61, tmp);
  pit_phase_c2(1);
}

void pcspkr_beep(uint32_t mst, uint16_t hz) {
  pcspkr_tone_on(hz);
  sleep(mst);
  pcspkr_tone_off();
}

int init_pcspkr() {
  outb(0x61, inb(0x61) | 0x1);
  return 0;
}
