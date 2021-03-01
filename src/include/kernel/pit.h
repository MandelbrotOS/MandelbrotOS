#ifndef __PIT_H__
#define __PIT_H__

#include <stdint.h>

extern uint64_t timer_ticks;

int init_timer();
void timer_phase(int hz);
void sleep(uint64_t milliseconds);

#endif // !__PIT_H__
