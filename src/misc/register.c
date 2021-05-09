#include <register.h>
#include <stddef.h>
#include <halt.h>

void getregisters(system_state_t is_system_dead)
{
	registers_t regs;
   	// I spent 3 hours trying to find a better method and failed
    	void *here = NULL;
    	asm inline ("movq %0, %%rax\n\t" : "=r" (regs.rax));
    	asm inline ("movq %0, %%rbx\n\t" : "=r" (regs.rbx));
    	asm inline ("movq %0, %%rcx\n\t" : "=r" (regs.rcx));
    	asm inline ("movq %0, %%rdx\n\t" : "=r" (regs.rdx));
    	asm inline ("movq %0, %%rsi\n\t" : "=r" (regs.rsi));
 	asm inline ("movq %0, %%rdi\n\t" : "=r" (regs.rdi));
    	asm inline ("movq %0, %%rbp\n\t" : "=r" (regs.rbp));
    	regs.rip = &here;
    	asm inline ("movq %0, %%r8\n\t" : "=r" (regs.r8));
    	asm inline ("movq %0, %%r9\n\t" : "=r" (regs.r9));
    	asm inline ("movq %0, %%r10\n\t" : "=r" (regs.r10));
    	asm inline ("movq %0, %%r11\n\t" : "=r" (regs.r11));
    	asm inline ("movq %0, %%r12\n\t" : "=r" (regs.r12));
    	asm inline ("movq %0, %%r13\n\t" : "=r" (regs.r13));
    	asm inline ("movq %0, %%r14\n\t" : "=r" (regs.r14));
    	asm inline ("movq %0, %%r15\n\t" : "=r" (regs.r15));
    	// NOTE: The value of RSP is an aproximation
    	void *rsp = NULL;
    	regs.rsp = (uint64_t)&rsp;
	printf("Register info:\n\n\rRAX: %#08x\r\nRBX: %#08x\r\nRCX: %#08x\r\nRDX: %#08x\r\nRSI: %#08x\r\nRDI: %#08x\r\nRBP: %#08x\r\nRSP: %#08x\r\nRIP: %#08x\r\n",
            regs.rax, regs.rbx, regs.rcx, regs.rdx, regs.rsi, regs.rdi, regs.rbp, regs.rsp, regs.rip);
    	char str[32];
    	printf("R8: %#08x\r\nR9: %#08x\r\nR10: %#08x\r\nR11: %#08x\r\nR12: %#08x\r\nR13: %#08x\r\nR14: %#08x\r\nR15: %#08x\r\n",
            regs.r8, regs.r9, regs.r10, regs.r11, regs.r12, regs.r13, regs.r14, regs.r15);
        if (is_system_dead)
        {
            /* I spent 2 hours on this stupid ascii art */
            printf("\r\nNow enjoy the nyan cat!\r\n\n");
            printf("+      o     +          +      o    +  o\r\n");
            printf("+             o     +        +    o        +\r\n");
            printf("o       +    o    +    o       +   o    +\r\n");
            printf("    o  +           +     +    O  o   o \r\n");
            printf("    +        o     o     +    o   o\r\n");
            printf("    -_-_-_-_-_-_-_,------,   o      +   *  +\r\n");
            printf("    _-_-_-_-_-_-_-|   /\\_/\\     +     o\r\n");
            printf("  o -_-_-_-_-_-_-~|__(_^__^)  +     +       +\r\n");
            printf("    _-_-_-_-_-_-_-   O   O        *   o   +\r\n");
            printf("    +      o      +        o     o\r\n");
            printf("        +      o    O    *   o     +     +\r\n");
            printf("  +     o        o         o      o     +\r\n");
            printf("   o         o           +\r\n");
            printf("   +        +      +     o        o      +\r\n");
            HALT();
        }
}
