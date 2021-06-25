#ifndef __GDT_H__
#define __GDT_H__

#include <stdint.h>

typedef struct gdt_pointer {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed)) gdt_pointer_t;

typedef struct gdt_entry {
  uint16_t limit;
  uint16_t low;
  uint8_t mid;
  uint8_t access;
  uint8_t granularity;
  uint8_t high;
} __attribute__((packed)) gdt_entry_t;

int init_gdt();

#endif
