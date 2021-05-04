#ifndef __HW_H__
#define __HW_H__

#include <stdint.h>

uint8_t inb(uint16_t _port);
void outb(uint16_t _port, uint8_t _data);

uint16_t inw(uint16_t _port);
void outw(uint16_t _port, uint16_t _data);

uint32_t ind(uint16_t _port);
void outd(uint16_t _port, uint16_t _data);

#endif // !__HW_H__
