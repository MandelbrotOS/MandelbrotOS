#ifndef __CPU_LOCALS_H__
#define __CPU_LOCALS_H__

#include <stddef.h>
#include <stdint.h>
#include <tasking/scheduler.h>

typedef struct cpu_locals {
  thread_t *current_thread;
  size_t task_count;
} cpu_locals_t;

#endif
