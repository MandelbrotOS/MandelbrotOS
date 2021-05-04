#include <stdint.h>

struct task {
    uintptr_t pc;
    uint64_t gp_regs[128];
};

struct task g_task[69];

void next_task() {
    
}