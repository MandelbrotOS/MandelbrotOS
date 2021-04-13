extern syscall_handler global syscall_asm_handler

    % macro pusha_no_rax 0 push rbx push rcx push rdx push rbp push rsi push rdi
        push rsp push r8 push r9 push r10 push r11 push r12 push r13 push r14
            push r15
    % endmacro

    % macro popa_no_rax 0 pop r15 pop r14 pop r13 pop r12 pop r11 pop r10 pop r9
        pop r8 pop rsp pop rdi pop rsi pop rbp pop rdx pop rcx pop rbx
    % endmacro

        syscall_asm_handler : cli pusha_no_rax mov rdi,
    rax mov rsi, rbx mov rbx, rdx mov rdx, rcx mov rcx, rbx mov ax,
    ds push rax mov ax, 0x10 mov ds, ax mov es, ax mov fs, ax mov gs,
    ax call syscall_handler pop rbx mov ds, bx mov es, bx mov fs, bx mov gs,
    bx popa_no_rax sti iretq
