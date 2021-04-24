#include <mm/heap.h>
#include <printf.h>
#include <string.h>
#include <tasking/tasking.h>
#include <boot/stivale2.h>
#include <mm/pmm.h>

extern void set_regs(uint64_t* pointer, uint64_t* stack);
extern void ppopa();
extern void ppusha();

void kidle(){
  while (1); 
}

void t2(){
  while (1)
    printf("Hello I am thread 2\r\n");
}

thread_t* current_thread;

thread_t* create_thread(uint64_t* function_ptr, char* name){
  thread_t *new_thread = kmalloc(sizeof(thread_t));
  if (new_thread == NULL)
    return NULL;

  new_thread->rip = function_ptr;

  memset(&new_thread->frame.registers, 0, sizeof(new_thread->frame.registers));

  new_thread->frame.registers.rsp = kmalloc(0x1000);
  if (new_thread->frame.registers.rsp == NULL){
    kfree(new_thread);
    return NULL;
  }

  new_thread->frame.registers.rsp += 0x1000;

  new_thread->name = name;

  return new_thread;
}

void destroy_thread(thread_t* thread){
  kfree((void *)thread->frame.registers.rsp);
  kfree(thread);
}

void add_thread(thread_t* to_add){
  __asm__ volatile("cli");
  to_add->next = current_thread->next;
  to_add->next->prev = to_add;
  to_add->prev = current_thread;
  current_thread->next = to_add;
  __asm__ volatile("sti");
}

void schedule(){
  __asm__ volatile("cli");
  /* ppusha(); */
  current_thread = current_thread->next;
  set_regs(current_thread->rip, current_thread->frame.gp_registers);
  /* ppopa(); */
  __asm__ volatile("sti");
}

int init_tasking(struct stivale2_struct_tag_smp_t *smp_info){
  current_thread = create_thread((uint64_t *)&kidle, "Thread1");
  current_thread->next = current_thread;
  current_thread->prev = current_thread;

  /* add_thread(create_thread((uint64_t*)&t2, "Thread 2"));  */

  /* schedule(); */

  printf("Problems have occured!\r\n");

  return 0;
}
