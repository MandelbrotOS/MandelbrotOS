#ifndef __PIT_H__
#define __PIT_H__

#include <stdint.h>

int init_pit();
void sleep(uint64_t milliseconds);
void timer_handler();

#endif
