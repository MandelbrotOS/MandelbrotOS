section .stivale2hdr
stivale2_header:
  .entry_point: dq 0 ; Default entry
  .stack: dq stack.top
  .flags: dq 0
  .tags: dq fb_tag

section .bss
stack:
  resb 4096
.top:

section .data
; Request a framebuffer (VBE in BIOS, GOP in UEFI)
fb_tag:
  .id: dq 0x3ecc1bc43d0f7971
  .next: dq fb_mtrr
  .fb_width: dw 0 ; Best size
  .fb_height: dw 0 ; Best size
  .fb_bpp: dw 32 ; Do not support less than 32 bpp

set_sse:
  ; Taken from OSDEV adjusted for 64 (Changed eax register to rax)
  mov rax, cr0
  and ax, 0xFFFB		;clear coprocessor emulation CR0.EM
  or ax, 0x2			;set coprocessor monitoring  CR0.MP
  mov cr0, rax
  mov rax, cr4
  or ax, 3 << 9		;set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
  mov cr4, rax
  ret

; Enable MTRR write-combining for the framebuffer to speed up
; writes into the framebuffer
fb_mtrr:
  .id: dq 0x4c7bb07731282e00
  .next: dq 0

gdt:
  .null: dq 0
  .code:
      dw 0         ; Limit low
      dw 0         ; Base low
      db 0         ; Base middle
      db 10011010b ; Access byte
      db 00100000b ; Flags and limit high
      db 0         ; Base high
  .data:
      dw 0         ; Limit low
      dw 0         ; Base low
      db 0         ; Base middle
      db 10010010b ; Access byte
      db 0         ; Flags and limit high
      db 0         ; Base high
.end:

gdt_register:
  .limit: dw gdt.end - gdt - 1
  .base: dq gdt

section .text
global _start
extern kernel_main
_start:
  lgdt [gdt_register]

mov rax, rsp

push 0x10
push rax
pushf
push 0x08
push .reload_segments
iretq

.reload_segments:
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  call set_sse

  call kernel_main
  hlt
