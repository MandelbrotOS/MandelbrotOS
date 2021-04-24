global ppusha
global ppopa
global set_regs

ppusha:
  push rax
  push rbx
  push rcx
  push rdx
  push rbp
  push rsi
  push rdi
  push rsp
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15
  ret

ppopa:
  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
  pop rsp
  pop rdi
  pop rsi
  pop rbp
  pop rdx
  pop rcx
  pop rbx
  pop rax
  ret
  
set_regs:
  mov rsp, rsi
  call rdi
  ret
