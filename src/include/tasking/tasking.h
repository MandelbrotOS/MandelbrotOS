#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stdint.h>
#include <boot/stivale2.h>

struct thread;
struct register_frame;

typedef struct register_frame {
  union {
    struct {
      uint64_t r15;
      uint64_t r14;
      uint64_t r13;
      uint64_t r12;
      uint64_t r11;
      uint64_t r10;
      uint64_t r9;
      uint64_t r8;
      uint64_t rsp;
      uint64_t rdi;
      uint64_t rsi;
      uint64_t rbp;
      uint64_t rdx;
      uint64_t rcx;
      uint64_t rbx;
      uint64_t rax;
      uint64_t rip;
      uint64_t cs;
    } registers;
    uint64_t gp_registers[18];
  };
} register_frame_t;

typedef struct thread {
  char* name;
  uint64_t* rip;
  uint64_t stacktop;
  uint64_t state;
  uint64_t pid;
  uint64_t priority;
  
  struct register_frame frame;

  struct thread* next;
  struct thread* prev;
} thread_t;

typedef struct {
  uint64_t thread_count;
  uint64_t current_thread;
} scheduler_t;

int init_tasking(struct stivale2_struct_tag_smp_t *smp_info);
void schedule();

#endif
