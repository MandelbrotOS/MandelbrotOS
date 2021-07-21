#ifndef __CPU_LOCALS_H__
#define __CPU_LOCALS_H__

#include <asm.h>
#include <stddef.h>
#include <stdint.h>
#include <tasking/scheduler.h>

typedef struct cpu_locals {
  size_t current_tid;
  size_t cpu_number;
  size_t lapic_id;
} cpu_locals_t;

static inline cpu_locals_t *get_locals() {
  return (cpu_locals_t *)rdmsr(0xc0000101);
}

static inline void set_locals(cpu_locals_t *local) {
  wrmsr(0xc0000101, (uint64_t)local);
}

#endif
