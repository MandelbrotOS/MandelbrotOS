#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <boot/stivale2.h>
#include <stdint.h>

struct thread;

typedef struct thread {
  char *name;
  uint64_t state;
  uint64_t pid;
  uint64_t priority;

  uint64_t *stack;

  struct thread *next;
  struct thread *prev;
} thread_t;

int init_tasking(struct stivale2_struct_tag_smp_t *smp_info);
extern void schedule();

#endif
