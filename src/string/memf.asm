memset:
    push  rbp
    cld
    mov   rbp, rsp                   
    mov   rax, rsi
    mov   rcx, rdx
    rep stosb
    pop   rbp
    ret

global memset

memset64:
    push  rbp
    cld
    mov   rbp, rsp                   
    mov   rax, rsi
    mov   rcx, rdx
    rep stosq
    pop   rbp
    ret

global memset64


memset32:
    push  rbp
    cld
    mov   rbp, rsp                   
    mov   rax, rsi
    mov   rcx, rdx
    rep stosd
    pop   rbp
    ret

global memset32

memset16:
    push  rbp
    cld
    mov   rbp, rsp                   
    mov   rax, rsi
    mov   rcx, rdx
    rep stosw
    pop   rbp
    ret

global memset32

memcpy:
    push  rbp
    cld
    mov   rbp, rsp                    
    mov   rcx, rdx
    rep   movsb
    pop   rbp
    ret

global memcpy

memcpy64:
    push  rbp
    cld
    mov   rbp, rsp                    
    mov   rcx, rdx
    rep   movsq
    pop   rbp
    ret

global memcpy64

memcpy32:
    push  rbp
    cld
    mov   rbp, rsp                    
    mov   rcx, rdx
    rep   movsd
    pop   rbp
    ret


global memcpy32

memcpy16:
    push  rbp
    cld
    mov   rbp, rsp                   
    mov   rcx, rdx
    rep   movsw
    pop   rbp
    ret


global memcpy16

