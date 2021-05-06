#include <kernel/idt.h>
#include <kernel/syscall.h>
#include <printf.h>
#include <stdint.h>
#include <string.h>

int64_t syscall_nop(uint64_t rbx, uint64_t rcx, uint64_t rdx) {
  printf("Hey, you called the NOP syscall!\r\n");
  return 0;
}

const syscall_t syscall_list[] = {(syscall_t){"nop", syscall_nop}};

void init_syscall(void) { idt_set_entry(&(idt[0x30]), 1, syscall_asm_handler); }

int64_t syscall_handler(uint64_t rax, uint64_t rbx, uint64_t rcx,
                        uint64_t rdx) {
  for (size_t i = 0; i < sizeof(syscall_list); i++)
    if (str_to_u64(syscall_list[i].name) == rax)
      return syscall_list[i].func(rbx, rcx, rdx);
  return -1;
}
