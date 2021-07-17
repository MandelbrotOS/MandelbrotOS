#include <boot/stivale2.h>
#include <drivers/apic.h>
#include <mm/kheap.h>
#include <mm/pmm.h>
#include <printf.h>
#include <registers.h>
#include <stdint.h>
#include <string.h>
#include <sys/irq.h>
#include <tasking/scheduler.h>

#define ALIVE 0
#define DEAD 1

struct stivale2_struct_tag_smp *smp;
size_t current_tid = 0;
scheduler_t sched;

void kidle() {
  printf("Reached kidle\r\n");
  while (1)
    ;
}

thread_t *create_kernel_thread(void *addr) {
  thread_t *thread = kcalloc(sizeof(thread_t));
  if (!thread)
    return NULL;

  thread->tid = current_tid++;
  thread->exit_state = -1;
  thread->state = ALIVE;
  thread->tid = current_tid++;
  thread->registers.cs = 0x08;
  thread->registers.ss = 0x10;
  thread->registers.rip = (uint64_t)addr;
  thread->registers.rflags = 0x202;
  thread->registers.rsp = (uint64_t)pcalloc(1) + PAGE_SIZE + PHYS_MEM_OFFSET;

  if (!thread->registers.rsp) {
    kfree(thread);
    return NULL;
  }

  return thread;
}

void exectute_thread() {}

void schedule(uint64_t rsp) {
}

void scheduler_init(struct stivale2_struct_tag_smp *smp_info) {
  smp = kmalloc(sizeof(struct stivale2_struct_tag_smp));
  memcpy(smp, smp_info, sizeof(struct stivale2_struct_tag_smp));

  sched.thread_count = 0;
  sched.task_queues = kmalloc(sizeof(task_queue_t) * smp_info->cpu_count);

  for (size_t i = 0; i < smp_info->cpu_count; i++) {
    sched.task_queues[i].thread_count = 0;
    sched.task_queues[i].current_thread = create_kernel_thread(kidle);
  }

  printf("We have %lu cores \r\n", smp_info->cpu_count);

  while (1)
    asm volatile("sti");
}
