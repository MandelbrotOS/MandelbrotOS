#include <hw.h>
#include <stdint.h>

// Read in byte from address
inline uint8_t inb(uint16_t _port) {
  unsigned char rv;
  __asm__ __volatile__("in %1, %0" : "=a"(rv) : "dN"(_port));
  return rv;
}

// Send out byte to address
inline void outb(uint16_t _port, uint8_t _data) {
  __asm__ __volatile__("out %1, %0" : : "dN"(_port), "a"(_data));
}

// Read in word from address
inline uint16_t inw(uint16_t _port) {
  uint16_t rv;
  __asm__ __volatile__("in %1, %0" : "=a"(rv) : "dN"(_port));
  return rv;
}

// Send out word to address
inline void outw(unsigned short _port, uint16_t _data) {
  __asm__ __volatile__("out %1, %0" : : "dN"(_port), "a"(_data));
}

// Read in dword from address
uint32_t ind(uint16_t _port) {
  uint32_t rv;
  __asm__ __volatile__("in %1, %0" : "=a"(rv) : "dN"(_port));
  return rv;
}

// Send out dword to address
void outd(uint16_t _port, uint16_t _data) {
  __asm__ __volatile__("out %1, %0" : : "dN"(_port), "a"(_data));
}
