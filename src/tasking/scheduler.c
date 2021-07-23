#include <acpi/acpi.h>
#include <asm.h>
#include <boot/stivale2.h>
#include <cpu_locals.h>
#include <drivers/apic.h>
#include <drivers/pit.h>
#include <drivers/serial.h>
#include <lock.h>
#include <mm/kheap.h>
#include <mm/pmm.h>
#include <printf.h>
#include <registers.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/irq.h>
#include <tasking/scheduler.h>

#define ALIVE 0
#define DEAD 1

size_t current_tid = 0;
size_t thread_count = 0;

thread_t *current_thread;

// TODO Fix sleep
void thread1() {
  while (1) {
    serial_print("Hello from thread 1\r\n");
    for (volatile size_t i = 0; i < 500000; i++)
      asm volatile("nop");
  }
}

// TODO Fix sleep
void thread2() {
  while (1) {
    serial_print("Hello from thread 2\r\n");
    for (volatile size_t i = 0; i < 500000; i++)
      asm volatile("nop");
  }
}

void k_idle() {
  while (1)
    ;
}

size_t create_kernel_thread(uintptr_t addr, char *name) {
  thread_t *thread = kmalloc(sizeof(thread_t));
  if (!thread)
    return -1;

  thread->tid = current_tid++;
  thread->exit_state = -1;
  thread->state = ALIVE;
  thread->name = name;
  thread->run_once = 0;
  thread->running = 0;
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

void schedule(uint64_t rsp) {
  MAKE_LOCK(sched_lock);

  current_thread->running = 0;

  if (current_thread->run_once)
    memcpy(&current_thread->registers, (registers_t *)rsp, sizeof(registers_t));
  else
    current_thread->run_once = 1;

  if (thread_count == 1)
    goto run_thread;

  for (size_t i = 0; i < thread_count; i++) {
    current_thread = current_thread->next;
    if (!current_thread->running && current_thread->state == ALIVE)
      goto run_thread;
  }

  current_thread = current_thread->next;

run_thread:
  current_thread->running = 1;
  lapic_eoi();
  UNLOCK(sched_lock);

  asm volatile("mov %0, %%rsp\n"
               "pop %%r15\n"
               "pop %%r14\n"
               "pop %%r13\n"
               "pop %%r12\n"
               "pop %%r11\n"
               "pop %%r10\n"
               "pop %%r9\n"
               "pop %%r8\n"
               "pop %%rbp\n"
               "pop %%rdi\n"
               "pop %%rsi\n"
               "pop %%rdx\n"
               "pop %%rcx\n"
               "pop %%rbx\n"
               "pop %%rax\n"
               "add $16, %%rsp\n"
               "iretq\n"
               :
               : "r"(&current_thread->registers)
               : "memory");
  serial_print("uh oh\r\n");
}

void scheduler_init(uintptr_t addr) {
  current_thread = kmalloc(sizeof(thread_t));

  *current_thread = (thread_t){
  	.tid = current_tid++,
  	.exit_state = -1,
  	.state = ALIVE,
  	.name = "k_init",
  	.run_once = 0,
  	.running = 0,
  	.registers = (registers_t){
	  .cs = 0x08,
	  .ss = 0x10,
	  .rip = (uint64_t)addr,
	  .rflags = 0x202,
	  .rsp = (uint64_t)pcalloc(1) + PAGE_SIZE + PHYS_MEM_OFFSET,
  	},
	.next = NULL,
  };

  thread_count++;
  current_thread->next = current_thread;

  create_kernel_thread((uintptr_t)thread1, "Thread 1");
  create_kernel_thread((uintptr_t)thread2, "Thread 2");

  irq_install_handler(SCHEDULE_REG - 32, schedule);

  while (1)
    asm volatile("sti");
}
