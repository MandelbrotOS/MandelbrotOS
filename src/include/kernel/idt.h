#ifndef __IDT_H__
#define __IDT_H__

#include <stdint.h>
#include <printf.h>

enum idt_attr {
  idt_a_present = 1 << 7,
  idt_a_ring_0 = 0 << 5,
  idt_a_ring_1 = 1 << 5,
  idt_a_ring_2 = 2 << 5,
  idt_a_ring_3 = 3 << 5,
  idt_a_type_interrupt = 0xE,
  idt_a_type_trap = 0xF
};
enum idt_vector {
  // --- Exceptions ---
  idt_v_divide_by_zero = 0x0,
  idt_v_debug = 0x1,
  idt_v_nmi = 0x2,
  idt_v_breakpoint = 0x3,
  idt_v_overflow = 0x4,
  idt_v_bound_range_exceeded = 0x5,
  idt_v_invalid_opcode = 0x6,
  idt_v_device_not_available = 0x7,
  idt_v_double_fault = 0x8,
  // [legacy][0x9] Coprocessor Segment Overrun
  idt_v_invalid_tss = 0xA,
  idt_v_segment_not_present = 0xB,
  idt_v_stack_segment_fault = 0xC,
  idt_v_general_protection_fault = 0xD,
  idt_v_page_fault = 0xE,
  // [reserved][0xF]
  idt_v_x87_floating_point_exception = 0x10,
  idt_v_alignment_check = 0x11,
  idt_v_machine_check = 0x12, // captcha? :)
  idt_v_simd_floating_point_exception = 0x13,
  idt_v_virtualization_exception = 0x14,
  // [reserved][0x15-0x1D]
  idt_v_security_exception = 0x1E
};

typedef struct idt_entry {
  uint16_t base_low;
  uint16_t sel;
  uint8_t ist;
  uint8_t flags;
  uint16_t base_mid;
  uint32_t base_high;
  uint32_t zero;
} __attribute__((packed)) idt_entry_t;
void set_entry(idt_entry_t *entry, void (*func)());

typedef struct idt_ptr {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed)) idt_ptr_t;

void set_entry(idt_entry_t *entry, void (*func)());
int init_idt();

#define IDT_HANDLER(NAME, CODE) \
  void NAME ## _idt_handler( /* a better way? */ \
    uint64_t _unused_rdi, uint64_t _unused_rsd, uint64_t _unused_rdx, \
    uint64_t _unused_rcx, uint64_t _unused_r8, uint64_t _unused_r9, \
    uint64_t r15, uint64_t r14, uint64_t r13, uint64_t r12, \
    uint64_t r11, uint64_t r10, uint64_t r9, uint64_t r8, \
    uint64_t rpb, uint64_t rdi, uint64_t rsi, uint64_t rdx, \
    uint64_t rcx, uint64_t rbx, uint64_t rax \
  ) { \
    /* it's shorter than adding __attribute__((unused)) to every argument */ \
    (void) _unused_rdi; (void) _unused_rsd; (void) _unused_rdx; \
    (void) _unused_rcx; (void) _unused_r8; (void) _unused_r9; \
    (void) r15; (void) r14; (void) r13; (void) r12; \
    (void) r11; (void) r10; (void) r9; (void) r8; \
    (void) rpb; (void) rdi; (void) rsi; (void) rdx; \
    (void) rcx; (void) rbx; (void) rax; \
    /* since it just casts, it will not produce any assembly */ \
    \
    CODE; \
  } \
  void NAME ## _asm_idt_handler();
#define STUB_IDT_HANDLER(NAME) \
  IDT_HANDLER(NAME, { \
    printf("\r\n\r\na " #NAME " exception happened!\r\n"); \
    asm volatile("cli; hlt"); \
  })

#endif // !__IDT_H__
