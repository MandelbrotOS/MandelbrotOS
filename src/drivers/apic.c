#include <asm.h>
#include <drivers/apic.h>
#include <mm/pmm.h>
#include <stdint.h>

#define LAPIC_REG_TIMER_SPURIOUS 0x0f0
#define LAPIC_REG_EOI 0x0b0
#define LAPIC_REG_ICR0 0x300
#define LAPIC_REG_ICR1 0x310
#define LAPIC_APIC_ID 0x20

uint32_t lapic_read(size_t reg) {
  return *((uint32_t *)(((rdmsr(0x1b) & 0xfffff000) + PHYS_MEM_OFFSET) + reg));
}

void lapic_write(size_t reg, uint32_t data) {
  *((uint32_t *)(((rdmsr(0x1b) & 0xfffff000) + PHYS_MEM_OFFSET) + reg)) = data;
}

void lapic_enable(uint8_t vect) {
  lapic_write(LAPIC_REG_TIMER_SPURIOUS,
              lapic_read(LAPIC_REG_TIMER_SPURIOUS) | (1 << 8) | vect);
}

uint8_t lapic_get_id() { return (uint8_t)(lapic_read(LAPIC_APIC_ID) >> 24); }

void lapic_eoi() { lapic_write(LAPIC_REG_EOI, 0); }

void lapic_send_ipi(uint8_t lapic_id, uint8_t vect) {
  lapic_write(LAPIC_REG_ICR1, lapic_id << 24);
  lapic_write(LAPIC_REG_ICR0, vect);
}

int init_lapic() {
  lapic_enable(0xf0);
  return 0;
}
