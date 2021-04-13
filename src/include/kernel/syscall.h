#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <stdint.h>

typedef struct syscall_t syscall_t;

struct syscall_t {
  char name[9];
  int64_t (*func)(uint64_t, uint64_t, uint64_t);
};

extern const syscall_t syscall_list[];

extern void syscall_asm_handler(void);

void init_syscall(void);

int64_t syscall_handler(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx);

#endif // !__SYSCALL_H__
