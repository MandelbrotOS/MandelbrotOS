#ifndef __HW_H__
#define __HW_H__

#include <stdint.h>

unsigned char inb(unsigned short _port);
void outb(unsigned short _port, unsigned char _data);
uint16_t inw(uint16_t _port);
void io_wait(void);

#endif // !__HW_H__