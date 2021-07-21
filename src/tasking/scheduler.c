#include <acpi/acpi.h>
#include <asm.h>
#include <boot/stivale2.h>
#include <cpu_locals.h>
#include <drivers/apic.h>
#include <lock.h>
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

size_t current_tid = 0;
size_t current_pid = 0;
size_t thread_count = 0;

thread_t *current_thread;

size_t create_kernel_thread(uintptr_t addr, char *name) {
  thread_t *thread = kmalloc(sizeof(thread_t));
  if (!thread)
    return -1;

  thread->tid = current_tid++;
  thread->exit_state = -1;
  thread->state = ALIVE;
  thread->tid = current_tid++;
  thread->name = name;
  thread->registers.cs = 0x08;
  thread->registers.ss = 0x10;
  thread->registers.rip = (uint64_t)addr;
  thread->registers.rflags = 0x202;
  thread->registers.rsp = (uint64_t)pcalloc(1) + PAGE_SIZE + PHYS_MEM_OFFSET;

  if (!thread->registers.rsp) {
    kfree(thread);
    return -1;
  }

  MAKE_LOCK(add_thread_lock);

  thread_count++;
  thread->next = current_thread->next;
  current_thread->next = thread;

  UNLOCK(add_thread_lock);

  return thread->tid;
}

void schedule(uint64_t rsp) { (void)rsp; }

void scheduler_init(uintptr_t addr) {
  thread_t *thread = kmalloc(sizeof(thread_t));

  thread->tid = current_tid++;
  thread->exit_state = -1;
  thread->state = ALIVE;
  thread->tid = current_tid++;
  thread->name = "k_init";
  thread->registers.cs = 0x08;
  thread->registers.ss = 0x10;
  thread->registers.rip = (uint64_t)addr;
  thread->registers.rflags = 0x202;
  thread->registers.rsp = (uint64_t)pcalloc(1) + PAGE_SIZE + PHYS_MEM_OFFSET;

  thread_count++;
  current_thread = thread;
  current_thread->next = current_thread;

  printf("Reached threading\r\n");

  irq_install_handler(SCHEDULE_REG - 32, schedule);

  while (1)
    asm volatile("sti");
}
