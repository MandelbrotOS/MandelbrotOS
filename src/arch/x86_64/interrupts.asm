; I Swear to God. I wrote such a nice assembler script and it pages faulted after evry few IRQ's.
; So anyway I stole this from: https://forum.osdev.org/viewtopic.php?f=1&t=27373
; One day I will revist this and make it look pretty

extern fault_handler
extern irq_handler

%macro pusha 0
      push   rax
      push   rbx
      push   rcx
      push   rdx
      push   rbp
      push   rsi
      push   rdi
      push   rsp
      push   r8
      push   r9
      push   r10
      push   r11
      push   r12
      push   r13
      push   r14
      push   r15
%endmacro

%macro popa 0
      pop       r15
      pop       r14
      pop       r13
      pop       r12
      pop       r11
      pop       r10
      pop       r9
      pop       r8
      pop       rsp
      pop       rdi
      pop       rsi
      pop       rbp
      pop       rdx
      pop       rcx
      pop       rbx
      pop       rax
%endmacro

%macro ISR_NOERRCODE 1
  [GLOBAL isr_%1]
  isr_%1:
    cli
    push byte 0
    push byte %1
    mov rdi, %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
  [GLOBAL isr_%1]
  isr_%1:
    cli
    push byte %1
    mov rdi, %1
    jmp isr_common_stub
%endmacro

%macro IRQ 2
  global irq_%1
  irq_%1:
    cli
    push byte 0
    push byte %2
    mov rdi, %1
    jmp irq_common_stub
%endmacro

isr_common_stub:
    pusha

    mov     ax, ds
    push    rax

    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    call fault_handler

    pop      rbx
    mov      ds, bx
    mov      es, bx
    mov      fs, bx
    mov      gs, bx


    popa
    add rsp,16
    sti
    iretq

irq_common_stub:

    pusha
    mov ax, ds
    push rax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq_handler
    pop rbx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa

    add rsp, 16
    sti
    iretq

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

IRQ   0,      32
IRQ   1,      33
IRQ   2,      34
IRQ   3,      35
IRQ   4,      36
IRQ   5,      37
IRQ   6,      38
IRQ   7,      39
IRQ   8,      40
IRQ   9,      41
IRQ  10,      42
IRQ  11,      43
IRQ  12,      44
IRQ  13,      45
IRQ  14,      46
IRQ  15,      47
