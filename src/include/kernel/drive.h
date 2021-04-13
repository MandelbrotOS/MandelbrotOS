#ifndef __DRIVE_H__
#define __DRIVE_H__

#include <kernel/device.h>
#include <stdint.h>

typedef struct drive_t drive_t;

struct drive_t {
    device_t* device;
};

#endif // !__DRIVE_H__
