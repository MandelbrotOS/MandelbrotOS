#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stddef.h>
#include <stdint.h>

#include <boot/stivale2.h>
#include <registers.h>

typedef struct thread {
  int state;
  int exit_state;
  size_t tid;
  size_t core;
  registers_t registers;
  struct thread *next;
} thread_t;

typedef struct task_queue {
  thread_t *current_thread;
  size_t thread_count;
} task_queue_t;

typedef struct scheduler {
  task_queue_t *task_queues;
  size_t thread_count;
} scheduler_t;

void schedule(uint64_t rsp);

void scheduler_init(struct stivale2_struct_tag_smp *smp);

#endif
