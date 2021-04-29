global get_cs
global get_flags
global schedule

extern c_handler

%macro pushaq 0
  push rsp
  push rax
  push rbx
  push rcx
  push rdx
  push rbp
  push rsi
  push rdi
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
  pop rdi
  pop rsi
  pop rbp
  pop rdx
  pop rcx
  pop rbx
  pop rax
  pop rsp
%endmacro

get_flags:
  pushfq
  pop rax
  ret

schedule:
  mov al, 0x20
  out 0x20, al

  cli
  pushaq

  call c_handler
  
  mov rsp, rax

  popaq
  sti
  
  ret
  ; iretq

