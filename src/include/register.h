#ifndef __REGISTER_H__
#define __REGISTER_H__ 1

#include <stdint.h>
#include <stdbool.h>

typedef uint64_t reg_t;

typedef struct
{
	// 64 bits registers
	reg_t rax;
	reg_t rbx;
	reg_t rcx;
	reg_t rdx;
	reg_t rsi;
	reg_t rdi;
	reg_t rbp;
	reg_t rsp;
	reg_t rip;
	reg_t r8;
	reg_t r9;
	reg_t r10;
	reg_t r11;
	reg_t r12;
	reg_t r13;
	reg_t r14;
	reg_t r15;
	// Flags
	reg_t flags;

} registers_t;

typedef enum {
    SYSTEM_STATE_DEAD,
    SYSTEM_STATE_ALIVE,
} system_state_t;

void getregisters(bool is_system_dead);

#endif /* __REGISTER_H__ */
