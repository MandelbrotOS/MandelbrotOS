#ifndef __APIC_H__
#define __APIC_H__

#include <stdint.h>

uint8_t lapic_get_id();
void lapic_eoi();

int init_lapic();

#endif
