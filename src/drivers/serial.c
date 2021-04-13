#include <drivers/serial.h>
#include <hw.h>
#include <kernel/device.h>
#include <mm/heap.h>
#include <stdint.h>

// Never use this function directly, use device_t.write instead.
int serial_write(void* device, void* data, uint64_t offset, uint64_t size)
{
    uint16_t port = *((uint16_t*)(device));
    uint8_t* bytes = (uint8_t*)(data);

    while (size--) {
        while (!(inb(port + 5) & 0x20))
            ;
        outb(port, *bytes++);
    }

    return 0; // Success
}

// Never use this function directly, use device_t.read instead.
int serial_read(void* device, void* data, uint64_t offset, uint64_t size)
{
    uint16_t port = *((uint16_t*)(device));
    uint8_t* bytes = (uint8_t*)(data);

    while (size--) {
        while (!(inb(port + 5) & 0x01))
            ;
        *bytes++ = inb(port);
    }

    return 0; // Success
}

// Never use this function directly, use device_t.get_size instead.
uint64_t serial_get_size(void* device)
{
    return 0; // Serial ports are huge...
}

int serial_device_init(device_t* device, uint16_t port)
{
    outb(port + 1, 0x00); // Disable interrupts
    outb(port + 3, 0x80);
    outb(port + 0, 0x01); // Set baud rate to 115200
    outb(port + 1, 0x00); // ^^^
    outb(port + 3, 0x03); // 8N1
    outb(port + 2, 0xC7);
    outb(port + 4, 0x0B);
    outb(port + 4, 0x1E); // Set loopback mode for testing
    outb(port + 0, 0xAE); // ^^^

    // Test if the serial port works
    if (inb(port) != 0xAE)
        return 1; // Fail

    // Guys, we did it. Now set normal mode
    outb(port + 4, 0x0F);

    // Now fill the device struct
    device->read = serial_read;
    device->write = serial_write;
    device->get_size = serial_get_size;

    // We only need to allocate the port number, no fancy structs
    uint16_t* port_ptr = device->device = kmalloc(2);
    *port_ptr = port;

    // And finally, return!
    return 0; // Success
}
