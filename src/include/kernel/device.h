#ifndef __DEVICE_H__
#define __DEVICE_H__

// TODO: Make this resizable
#define MAX_DEVICES 256

#include <stdint.h>

typedef struct device_t device_t;

struct device_t {
    char name[5];
    int present; // Only used by the device allocator

    void* device;

    int (*read)(void* device, void* data, uint64_t offset, uint64_t size);
    int (*write)(void* device, void* data, uint64_t offset, uint64_t size);
    uint64_t (*get_size)(void* device);
};

extern device_t device_list[MAX_DEVICES];

void device_init(void);
device_t* device_add(const char* name);
void device_remove(device_t* device);

#endif // !__DEVICE_H__
