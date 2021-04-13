#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <kernel/device.h>
#include <stdint.h>

int serial_device_init(device_t* device, uint16_t port);

#endif // !__SERIAL_H__
