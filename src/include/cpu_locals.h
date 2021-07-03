#ifndef __CPU_LOCALS_H__
#define __CPU_LOCALS_H__

#include <stdint.h>
#include <tasking/scheduler.h>
#include <stddef.h>

typedef struct cpu_locals {
  uint8_t lapic_id;
  size_t current_pid;
  size_t current_tid;
} cpu_locals_t;

#endif
