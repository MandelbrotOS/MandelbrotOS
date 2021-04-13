#include <drivers/ata.h>
#include <hw.h>

void ata_wait_bsy(ata_device_t *device)
{
  while (inb(device->base_port + ATA_PORT_STATUS) & ATA_STATUS_BSY)
    ;
}

bool ata_wait_drq(ata_device_t *device)
{
  while (true) {
    uint8_t status = inb(device->base_port + ATA_PORT_STATUS);
    if (status & ATA_STATUS_DRQ)
      return true;
    if (status & ATA_STATUS_ERR || status & ATA_STATUS_DF)
      return false;
  }
}

void ata_pio_send_lba48(ata_device_t *device, uint64_t lba, uint16_t sectors)
{
  outb(device->base_port + ATA_PORT_SECTOR_COUNT, sectors >> 8);
  outb(device->base_port + ATA_PORT_LBA_LO, lba >> 24);
  outb(device->base_port + ATA_PORT_LBA_MID, lba >> 32);
  outb(device->base_port + ATA_PORT_LBA_HI, lba >> 40);
  outb(device->base_port + ATA_PORT_SECTOR_COUNT, sectors);
  outb(device->base_port + ATA_PORT_LBA_LO, lba);
  outb(device->base_port + ATA_PORT_LBA_MID, lba >> 8);
  outb(device->base_port + ATA_PORT_LBA_HI, lba >> 16);
}

// FIXME: This is completely broken
bool ata_pio_present(ata_device_t *device, bool slave)
{
  // FIXME: This piece breaks it
  // if (! slave) outb(device->base_port + ATA_PORT_DRIVE_HEAD, 0xA0);
  // else outb(device->base_port + ATA_PORT_DRIVE_HEAD, 0xB0);

  for (int i = 0; i < 4; ++i) {
    inb(device->base_control_port + ATA_CONTROL_STATUS);
  }

  // FIXME: This one breaks it too
  // outb(device->base_port + ATA_PORT_SECTOR_COUNT, 0);
  // outb(device->base_port + ATA_PORT_LBA_LO, 0);
  // outb(device->base_port + ATA_PORT_LBA_MID, 0);
  // outb(device->base_port + ATA_PORT_LBA_HI, 0);
  // outb(device->base_port + ATA_PORT_COMMAND, ATA_COMMAND_IDENTIFY);

  uint8_t status = inb(device->base_port + ATA_PORT_STATUS);
  return status != 0;
}

// Read x sectors from device
int ata_pio_read_lba(
    ata_device_t *device, uint16_t *data, uint64_t lba, uint64_t sectors)
{
  uint16_t *target = (uint16_t *)(data);

  while (sectors) {
    uint16_t sectors_lo = (uint16_t)(sectors > 0xFFFF ? 0xFFFF : sectors);

    ata_wait_bsy(device);
    ata_pio_send_lba48(device, lba, sectors_lo);

    outb(device->base_port + ATA_PORT_COMMAND, ATA_COMMAND_READ_SECTORS_EXT);

    for (uint16_t i = 0; i < sectors_lo; i++) {
      ata_wait_bsy(device);

      if (!ata_wait_drq(device)) {
        uint8_t status = inb(device->base_port + ATA_PORT_STATUS);

        if (status & ATA_STATUS_ERR)
          return 1;
        if (status & ATA_STATUS_DF)
          return 2;
      }

      for (int j = 0; j < 256; j++)
        target[j] = inw(device->base_port + ATA_PORT_DATA);

      target += 256;

      lba += sectors_lo;
      sectors -= sectors_lo;
    }
  }

  return 0;
}

// Write data to device
int ata_pio_write_lba(
    ata_device_t *device, void *data, uint64_t lba, uint64_t sectors)
{
  uint16_t *words = (uint16_t *)(data);

  while (sectors) {
    uint16_t sectors_lo = (uint16_t)(sectors > 0xFFFF ? 0xFFFF : sectors);

    ata_wait_bsy(device);
    ata_pio_send_lba48(device, lba, sectors_lo);

    outb(device->base_port + ATA_PORT_COMMAND, ATA_COMMAND_WRITE_SECTORS_EXT);

    for (uint16_t i = 0; i < sectors_lo; i++) {
      ata_wait_bsy(device);

      if (!ata_wait_drq(device)) {
        uint8_t status = inb(device->base_port + ATA_PORT_STATUS);

        if (status & ATA_STATUS_ERR)
          return 1;
        if (status & ATA_STATUS_DF)
          return 2;
      }

      for (int j = 0; j < 256; j++)
        outw(device->base_port + ATA_PORT_DATA, words[j]);

      words += 256;

      lba += sectors_lo;
      sectors -= sectors_lo;
    }
  }

  return 0;
}

// Never use this function directly, use device_t.read instead.
int ata_pio_read(void *device, void *data, uint64_t offset, uint64_t size)
{
  // TODO: Read size/512, return size bytes ignoring the rest

  return 1; // Success
}

// Never use this function directly, use device_t.write instead.
int ata_pio_write(void *device, void *data, uint64_t offset, uint64_t size)
{
  // TODO: Write size/512 sectors with size bytes leaving the
  // rest intact

  return 1; // Success
}

void ata_pio_device_init(
    ata_device_t *device, uint16_t base_port, uint16_t base_control_port)
{
  // FIXME: The afformentioned bottom code is not working.
  // The struct does not have the required params. Can segfaultdev please
  // look into this?
  // device->read = ata_pio_read;
  // device->write = ata_pio_write;

  // TODO: device->get_size = ...;

  // TODO: Allocating enough bytes for ata_device_t on device
}
