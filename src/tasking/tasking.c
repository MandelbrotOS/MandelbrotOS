#include <boot/stivale2.h>
#include <mm/heap.h>
#include <mm/pmm.h>
#include <printf.h>
#include <string.h>
#include <tasking/tasking.h>

void kidle() {
  while (1)
    printf("Hello I am thread 1\r\n");
}

void t2() {
  while (1)
    printf("Hello I am thread 2\r\n");
}

#define THREAD_STACK_SIZE (0x2000)
#define USERSPACE_CODE_SEGMENT (0x8)
#define REGISTER_COUNT (15)
#define THREAD_FLAGS (0x202)

thread_t *create_thread(uint64_t *function_ptr, char *name) {
  thread_t *new_thread = kmalloc(sizeof(thread_t));
  if (!new_thread)
    return NULL;

  new_thread->stack = (uintptr_t)kmalloc(THREAD_STACK_SIZE);
  
  if (!new_thread->stack) {
    kfree(new_thread);
    return NULL;
  }

  memset((void*)new_thread->stack - THREAD_STACK_SIZE, 0, THREAD_STACK_SIZE);

  new_thread->stack -= sizeof(uint64_t);
  *((uint64_t *)(new_thread->stack)) = THREAD_FLAGS;
  new_thread->stack -= sizeof(uint64_t);
  *((uint64_t *)(new_thread->stack)) = USERSPACE_CODE_SEGMENT;
  new_thread->stack -= sizeof(uint64_t);
  *((uint64_t *)(new_thread->stack)) = function_ptr;
  new_thread->stack -= sizeof(uint64_t);
  *((uint64_t *)(new_thread->stack)) = new_thread->stack;

  new_thread->stack -= sizeof(uint64_t) * REGISTER_COUNT;
  
  new_thread->name = name;

  return new_thread;
}

void destroy_thread(thread_t *thread) {
  kfree((void *)thread->stack);
  kfree(thread);
}

void add_thread(thread_t *to_add) {
  __asm__ volatile("cli");
  to_add->next = current_thread->next;
  to_add->next->prev = to_add;
  to_add->prev = current_thread;
  current_thread->next = to_add;
  __asm__ volatile("sti");
}

uintptr_t c_handler() {
  current_thread = current_thread->next;
  printf("Set the current thread\r\n");
  return current_thread->stack;
}

int init_tasking(struct stivale2_struct_tag_smp_t *smp_info) {
  current_thread = create_thread((uint64_t *)&kidle, "Thread1");
  current_thread->next = current_thread;
  current_thread->prev = current_thread;

  add_thread(create_thread((uint64_t*)&t2, "Thread2"));

  printf("Problems have occured!\r\n");

  return 0;
}
