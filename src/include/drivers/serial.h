#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdint.h>

#define SERIAL_COM1_BASE (0x3F8)
#define SERIAL_COM2_BASE (0x2F8)
#define SERIAL_COM3_BASE (0x3E8)
#define SERIAL_COM4_BASE (0x2E8)

int init_serial();
void serial_print(char *buf);

#endif
