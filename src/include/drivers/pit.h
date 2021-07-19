#ifndef __PIT_H__
#define __PIT_H__

#include <stdint.h>

void sleep(uint64_t ticks);
int init_pit();

#endif
