#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stddef.h>
#include <stdint.h>

#include <boot/stivale2.h>
#include <registers.h>

typedef struct thread {
  char *name;
  int state;
  int exit_state;
  size_t tid;
  uint64_t cr3;
  registers_t registers;
  struct thread *next;
} thread_t;

void schedule(uint64_t rsp);

void scheduler_init(uintptr_t addr);

#endif
