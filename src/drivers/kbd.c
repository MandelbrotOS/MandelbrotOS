#include <drivers/kbd.h>
#include <kernel/irq.h>

uint8_t kbd_ctrl_read_status() {
	return inb(KBD_CTRL_STATS_REG);
}

void kbd_ctrl_send_cmd(uint8_t cmd) {
	while(1)
		if((kbd_ctrl_read_status() & KBD_CTRL_STATS_MASK_IN_BUF) == 0) break;
	outb(KBD_CTRL_CMD_REG, cmd);
}

uint8_t kbd_enc_read_buf() {
	return inb(KBD_ENC_INPUT_BUF);
}

void kbd_enc_send_cmd(uint8_t cmd) {
	while(1)
		if((kbd_ctrl_read_status() & KBD_CTRL_STATS_MASK_IN_BUF) == 0) break;
	outb(KBD_ENC_CMD_REG, cmd);
}

void kbd_handler() {
	kbd_ctrl_read_status();
}

int init_kbd (int irq) {
	irq_install_handler(1, kbd_handler);
	return 0;
}

void getline(char string[], int len) {
	//int len = sizeof(string)/sizeof(string[0]);
	uint8_t i = 0;
	char temp = 0;

	while(i < len && temp != 0x0D) {
		temp = getchar();
		if ((temp >= 0 && temp < 128) && temp != 0x0D) {
			if (temp >= 22 && temp <= 127) {
				printf("%c", temp);
				string[i] = temp;
				i ++;
			}
		}
	}
}

char getchar() {
  uint8_t code = 0;
  uint8_t key = 0;
  while(1) {
    if (kbd_ctrl_read_status() & KBD_CTRL_STATS_MASK_OUT_BUF) {
      code = kbd_enc_read_buf();
      if(code <= 0x58) {
        key = _kkybrd_scancode_std[code];
        break;
      }
    }
  }
  return key;
}
