#ifndef __PIT_H__
#define __PIT_H__

#include <stdint.h>

void init_pit();
void sleep(uint64_t milliseconds);

#endif
