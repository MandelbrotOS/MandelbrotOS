#include <asm.h>
#include <drivers/serial.h>
#include <lock.h>
#include <stddef.h>
#include <stdint.h>

#define SERIAL_DATA_PORT(BASE) (BASE)
#define SERIAL_FIFO_COMMAND_PORT(BASE) (BASE + 2)
#define SERIAL_LINE_COMMAND_PORT(BASE) (BASE + 3)
#define SERIAL_MODEM_COMMAND_PORT(BASE) (BASE + 4)
#define SERIAL_LINE_STATUS(BASE) (BASE + 5)

#define SERIAL_LINE_ENABLE_DLAB (0x80)

void serial_conf_baud_rate(uint16_t base, uint16_t divisor) {
  outb(SERIAL_LINE_COMMAND_PORT(base), SERIAL_LINE_ENABLE_DLAB);

  outb(SERIAL_DATA_PORT(base), (divisor >> 8) & 0x00FF);
  outb(SERIAL_DATA_PORT(base) + 1, divisor & 0x00FF);
}

void serial_conf_line(uint16_t base) {
  outb(SERIAL_LINE_COMMAND_PORT(base), 0x03);
}

void serial_conf_buff(uint16_t base) {
  outb(SERIAL_FIFO_COMMAND_PORT(base), 0xC7);
}

void serial_conf_modem(uint16_t base) {
  outb(SERIAL_MODEM_COMMAND_PORT(base), 0x0B);
}

int init_serial_port(uint16_t base, uint16_t divisor) {
  outb(SERIAL_DATA_PORT(base) + 1, 0x00);
  serial_conf_baud_rate(base, divisor);
  serial_conf_line(base);
  serial_conf_buff(base);
  serial_conf_modem(base);
  return 0;
}

int serial_is_transmit_fifo_empty(uint32_t base) {
  return inb(SERIAL_LINE_STATUS(base)) & 20;
}

void serial_write_byte_port(uint16_t base, char byte) {
  outb(SERIAL_DATA_PORT(base), byte);
}

void serial_write_port(uint16_t base, char *buf, size_t len) {
  while (!serial_is_transmit_fifo_empty(base))
    ;
  for (uint32_t i = 0; i < len; i++)
    serial_write_byte_port(base, buf[i]);
}

void serial_print_port(uint16_t base, char *buf) {
  while (serial_is_transmit_fifo_empty(base))
    ;
  for (uint32_t i = 0; buf[i]; i++)
    serial_write_byte_port(base, buf[i]);
}

int init_serial() {
  init_serial_port(SERIAL_COM1_BASE, 3);
  init_serial_port(SERIAL_COM2_BASE, 3);
  init_serial_port(SERIAL_COM3_BASE, 3);
  init_serial_port(SERIAL_COM4_BASE, 3);

  return 0;
}

void serial_print(char *buf) {
  MAKE_LOCK(serial_print_lock);
  serial_print_port(SERIAL_COM1_BASE, buf);
  /* serial_print_port(SERIAL_COM2_BASE, buf); */
  /* serial_print_port(SERIAL_COM3_BASE, buf); */
  /* serial_print_port(SERIAL_COM4_BASE, buf); */
  UNLOCK(serial_print_lock);
}
