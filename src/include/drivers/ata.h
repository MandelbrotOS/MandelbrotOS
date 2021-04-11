#ifndef __ATA_H__
#define __ATA_H__

#include <stdbool.h>
#include <stdint.h>
#include <kernel/device.h>

typedef struct ata_device_t ata_device_t;

struct ata_device_t {
  uint16_t base_port;
  uint16_t base_control_port;
};

bool ata_pio_present(ata_device_t *device, bool slave);

int ata_pio_read_lba(ata_device_t *device, uint16_t *data, uint64_t lba,
                     uint64_t sectors);
int ata_pio_write_lba(ata_device_t *device, void *data, uint64_t lba,
                      uint64_t sectors);

int ata_pio_device_init(device_t *device, uint16_t base_port,
                        uint16_t base_control_port);

#define ATA_PRIMARY_PORT 0x1F0
#define ATA_SECONDARY_PORT 0x170

#define ATA_PRIMARY_CONTROL_PORT 0x3F6
#define ATA_SECONDARY_CONTROL_PORT 0x376

#define ATA_PORT_DATA 0
#define ATA_PORT_ERROR 1
#define ATA_PORT_FEATURES 1
#define ATA_PORT_SECTOR_COUNT 2
#define ATA_PORT_LBA_LO 3
#define ATA_PORT_LBA_MID 4
#define ATA_PORT_LBA_HI 5
#define ATA_PORT_DRIVE_HEAD 6
#define ATA_PORT_STATUS 7
#define ATA_PORT_COMMAND 7

#define ATA_CONTROL_STATUS 0
#define ATA_CONTROL_DEVICE_CONTROL 0
#define ATA_CONTROL_DRIVE_ADDRESS 2

#define ATA_STATUS_ERR 1
#define ATA_STATUS_IDX 1 << 1
#define ATA_STATUS_CORR 1 << 2
#define ATA_STATUS_DRQ 1 << 3
#define ATA_STATUS_SRV 1 << 4
#define ATA_STATUS_DF 1 << 5
#define ATA_STATUS_RDY 1 << 6
#define ATA_STATUS_BSY 1 << 7

#define ATA_COMMAND_IDENTIFY 0xEC
#define ATA_COMMAND_READ_SECTORS_EXT 0x24
#define ATA_COMMAND_WRITE_SECTORS_EXT 0x34

#endif // !__ATA_H__
