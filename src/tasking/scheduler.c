#include <boot/stivale2.h>
#include <drivers/apic.h>
#include <printf.h>
#include <registers.h>
#include <stdint.h>
#include <sys/irq.h>
#include <tasking/scheduler.h>
#include <mm/pmm.h>

#define ALIVE 0
#define DEAD 1

struct stivale2_struct_tag_smp *smp;
size_t current_tid = 0;

void kidle() {
  printf("Reached kidle\r\n");
  while (1)
    ;
}

thread_t create_kernel_thread(void* addr) {
  thread_t thread = {0};
  thread.tid = current_tid++;
  thread.exit_state = -1;
  thread.state = ALIVE;
  thread.tid = current_tid++;
  thread.registers.cs = 0x08;
  thread.registers.ss = 0x10;
  thread.registers.rip = (uint64_t)addr;
  thread.registers.rflags = 0x202;
  thread.registers.rsp = (uint64_t)pcalloc(1) + PAGE_SIZE + PHYS_MEM_OFFSET;

  return thread;
}

void exectute_thread() {
  /* asm volatile("mov %0, %%rsp\n" */
               /* "pop %%r15\n" */
               /* "pop %%r14\n" */
               /* "pop %%r13\n" */
               /* "pop %%r12\n" */
               /* "pop %%r11\n" */
               /* "pop %%r10\n" */
               /* "pop %%r9\n" */
               /* "pop %%r8\n" */
               /* "pop %%rbp\n" */
               /* "pop %%rdi\n" */
               /* "pop %%rsi\n" */
               /* "pop %%rdx\n" */
               /* "pop %%rcx\n" */
               /* "pop %%rbx\n" */
               /* "pop %%rax\n" */
               /* "add $16, %%rsp\n" // reserved1 and reserved2 */
               /* "iretq\n" ::"r"(get_locals()->current_thread->registers) */
               /* : "memory"); */
}

void schedule(uint64_t rsp) {
  /* for (size_t i = 0; i < smp->cpu_count; i++) { */
    /* cpu_locals_t* local = get_locals(); */
    /* local->current_thread->registers = *(registers_t *)rsp; */
    /* local->current_thread = local->current_thread->next; */
    /* smp->smp_info[i].goto_address = (uint64_t)exectute_thread; */
  /* } */
}

void scheduler_init(struct stivale2_struct_tag_smp *smp) {
  /* smp = smp; */
  printf("We have %lu cores \r\n", smp->cpu_count);
  while (1)
    asm volatile("sti");
}

