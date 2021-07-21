extern c_isr_handler
extern c_irq_handler

%macro pushaq 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popaq 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

isr_stub:
  pushaq

  mov rdi, [rsp + 120]
  mov rsi, rsp
  call c_isr_handler
  
  popaq
  
  add rsp, 16

  iretq

irq_stub:
  pushaq

  mov rdi, [rsp + 120]
  mov rsi, rsp
  call c_irq_handler
  
  popaq
  
  add rsp, 16

  iretq

%macro ISR 1
  global isr%1
  isr%1:
    cli
    push 0
    push %1
    jmp isr_stub
%endmacro

%macro ISR_ERR 1
  global isr%1
  isr%1:
    cli
    push %1
    jmp isr_stub
%endmacro

%macro IRQ 1
  global irq%1
  irq%1:
    cli
    push 0
    push %1
    jmp irq_stub
%endmacro

ISR 0
ISR 1 
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR_ERR 8
ISR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13 
ISR_ERR 14 
ISR 15 
ISR 16 
ISR_ERR 17
ISR 18 
ISR 19 
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15
IRQ 16

