global memset
global memset16
global memset32
global memset64

global memcpy
global memcpy16
global memcpy32
global memcpy64

memset:
    push  rbp
    cld
    mov   rbp, rsp                   
    mov   rax, rsi
    mov   rcx, rdx
    rep stosb
    pop   rbp
    ret

memset16:
    push  rbp
    cld
    mov   rbp, rsp                   
    mov   rax, rsi
    mov   rcx, rdx
    rep stosw
    pop   rbp
    ret

memset32:
    push  rbp
    cld
    mov   rbp, rsp                   
    mov   rax, rsi
    mov   rcx, rdx
    rep stosd
    pop   rbp
    ret

memset64:
    push  rbp
    cld
    mov   rbp, rsp                   
    mov   rax, rsi
    mov   rcx, rdx
    rep stosq
    pop   rbp
    ret

memcpy:
    push  rbp
    cld
    mov   rbp, rsp                    
    mov   rcx, rdx
    rep   movsb
    pop   rbp
    ret

memcpy16:
    push  rbp
    cld
    mov   rbp, rsp                   
    mov   rcx, rdx
    rep   movsw
    pop   rbp
    ret

memcpy32:
    push  rbp
    cld
    mov   rbp, rsp                    
    mov   rcx, rdx
    rep   movsd
    pop   rbp
    ret

memcpy64:
    push  rbp
    cld
    mov   rbp, rsp                    
    mov   rcx, rdx
    rep   movsq
    pop   rbp
    ret
