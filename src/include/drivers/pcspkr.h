#ifndef __PCSPKR_H__
#define __PCSPKR_H__

#include <stdint.h>

int init_pcspkr();
void pcspkr_tone_on(uint32_t hz);
void pcspkr_tone_off();
void pcspkr_beep(uint32_t mst);
void pit_phase_c2(uint32_t hz);

#endif
