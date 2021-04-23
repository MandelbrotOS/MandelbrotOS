#include <mm/heap.h>
#include <printf.h>
#include <string.h>
#include <tasking/tasking.h>
#include <boot/stivale2.h>
#include <mm/pmm.h>

extern int set_regs(int old_stack, int pointer, int cr3, int new_stack);
extern void ppopa();
extern void ppusha();

void t1(){
  printf("Hello I am thread 1\r\n");
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
  kfree(thread->frame.registers.rsp);
  kfree(thread);
}

void add_thread(thread_t* to_add){
  to_add->next = current_thread->next;
  to_add->next->prev = to_add;
  to_add->prev = current_thread;
  current_thread->next = to_add;
}

void schedule(){

}

int init_tasking(struct stivale2_struct_tag_smp_t *smp_info){
  current_thread = create_thread((uint64_t *)&t1, "Thread1");
  current_thread->next = current_thread;
  current_thread->prev = current_thread;

  ppusha();
  set_regs((int*)0xded, (int)current_thread->rip, current_thread->cr3, current_thread->frame.gp_registers);
  ppopa();

  return 0;
}
