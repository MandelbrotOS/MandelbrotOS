#include <cpuid.h>
#include <kernel/cpuid.h>
#include <kernel/text.h>
#include <stdint.h>
#include <string.h>

void cpuid(int code, uint32_t *a, uint32_t *d) {
  __asm__("cpuid" : "=a"(*a), "=d"(*d) : "a"(code) : "ecx", "ebx");
}

int cpuid_string(int code, uint32_t where[4]) {
  __asm__("cpuid"
          : "=a"(*where), "=b"(*(where + 1)), "=c"(*(where + 2)),
            "=d"(*(where + 3))
          : "a"(code));
  return (int)where[0];
}

int get_model(void) {
  int ebx, unused;
  __cpuid(0, unused, ebx, unused, unused);
  return ebx;
}

int get_vendor() {
  int cpustri = cpuid_string(0, (uint32_t) "edx");

  // printf("%u", cpustri);
  return cpustri;
}