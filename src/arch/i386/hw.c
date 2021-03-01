#include <hw.h>

// From Bran's OSDEV series because I can't be bothered to write my own.

unsigned char inb(unsigned short _port) {
  unsigned char rv;
  __asm__ __volatile__("inb %1, %0" : "=a"(rv) : "dN"(_port));
  return rv;
}

void outb(unsigned short _port, unsigned char _data) {
  __asm__ __volatile__("outb %1, %0" : : "dN"(_port), "a"(_data));
}

uint16_t inw(uint16_t _port) {
  uint16_t rv;
  __asm__ volatile("inw %1, %0" : "=a"(rv) : "dN"(_port));
  return rv;
}

void io_wait(void) {
  __asm__ volatile("jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:");
}