#include <asm.h>
#include <drivers/kbd.h>
#include <fb/fb.h>
#include <sys/irq.h>

static int _kkybrd_scancode_std[] = {

    //! key			scancode
    KEY_UNKNOWN,      // 0
    KEY_ESCAPE,       // 1
    KEY_1,            // 2
    KEY_2,            // 3
    KEY_3,            // 4
    KEY_4,            // 5
    KEY_5,            // 6
    KEY_6,            // 7
    KEY_7,            // 8
    KEY_8,            // 9
    KEY_9,            // 0xa
    KEY_0,            // 0xb
    KEY_MINUS,        // 0xc
    KEY_EQUAL,        // 0xd
    KEY_BACKSPACE,    // 0xe
    KEY_TAB,          // 0xf
    KEY_Q,            // 0x10
    KEY_W,            // 0x11
    KEY_E,            // 0x12
    KEY_R,            // 0x13
    KEY_T,            // 0x14
    KEY_Y,            // 0x15
    KEY_U,            // 0x16
    KEY_I,            // 0x17
    KEY_O,            // 0x18
    KEY_P,            // 0x19
    KEY_LEFTBRACKET,  // 0x1a
    KEY_RIGHTBRACKET, // 0x1b
    KEY_RETURN,       // 0x1c
    KEY_LCTRL,        // 0x1d
    KEY_A,            // 0x1e
    KEY_S,            // 0x1f
    KEY_D,            // 0x20
    KEY_F,            // 0x21
    KEY_G,            // 0x22
    KEY_H,            // 0x23
    KEY_J,            // 0x24
    KEY_K,            // 0x25
    KEY_L,            // 0x26
    KEY_SEMICOLON,    // 0x27
    KEY_QUOTE,        // 0x28
    KEY_GRAVE,        // 0x29
    KEY_LSHIFT,       // 0x2a
    KEY_BACKSLASH,    // 0x2b
    KEY_Z,            // 0x2c
    KEY_X,            // 0x2d
    KEY_C,            // 0x2e
    KEY_V,            // 0x2f
    KEY_B,            // 0x30
    KEY_N,            // 0x31
    KEY_M,            // 0x32
    KEY_COMMA,        // 0x33
    KEY_DOT,          // 0x34
    KEY_SLASH,        // 0x35
    KEY_RSHIFT,       // 0x36
    KEY_KP_ASTERISK,  // 0x37
    KEY_RALT,         // 0x38
    KEY_SPACE,        // 0x39
    KEY_CAPSLOCK,     // 0x3a
    KEY_F1,           // 0x3b
    KEY_F2,           // 0x3c
    KEY_F3,           // 0x3d
    KEY_F4,           // 0x3e
    KEY_F5,           // 0x3f
    KEY_F6,           // 0x40
    KEY_F7,           // 0x41
    KEY_F8,           // 0x42
    KEY_F9,           // 0x43
    KEY_F10,          // 0x44
    KEY_KP_NUMLOCK,   // 0x45
    KEY_SCROLLLOCK,   // 0x46
    KEY_HOME,         // 0x47
    KEY_UP,           // 0x48	//keypad up arrow
    KEY_PAGEUP,       // 0x49
    KEY_KP_2,         // 0x50	//keypad down arrow
    KEY_KP_3,         // 0x51	//keypad page down
    KEY_KP_0,         // 0x52	//keypad insert key
    KEY_KP_DECIMAL,   // 0x53	//keypad delete key
    KEY_UNKNOWN,      // 0x54
    KEY_UNKNOWN,      // 0x55
    KEY_UNKNOWN,      // 0x56
    KEY_F11,          // 0x57
    KEY_F12           // 0x58
};

uint8_t kbd_ctrl_read_status() { return inb(KBD_CTRL_STATS_REG); }

void kbd_ctrl_send_cmd(uint8_t cmd) {
  while (1)
    if ((kbd_ctrl_read_status() & KBD_CTRL_STATS_MASK_IN_BUF) == 0)
      break;
  outb(KBD_CTRL_CMD_REG, cmd);
}

uint8_t kbd_enc_read_buf() { return inb(KBD_ENC_INPUT_BUF); }

void kbd_enc_send_cmd(uint8_t cmd) {
  while (1)
    if ((kbd_ctrl_read_status() & KBD_CTRL_STATS_MASK_IN_BUF) == 0)
      break;
  outb(KBD_ENC_CMD_REG, cmd);
}

void kbd_handler() { kbd_ctrl_read_status(); }

int init_kbd() {
  irq_install_handler(1, kbd_handler);
  return 0;
}

void getline(char *string, int len) {
  char temp = 0;
  for (int i = 0; i < len && temp != 0x0D; i++) {
    temp = getchar();
    if (temp >= 0 && temp != 0x0D) {
      putchar(temp);
      string[i] = temp;
    }
  }
  putchar('\n');
  putchar('\r');
}

is_caps = 0;

char getchar() {
  uint8_t code = 0;
  uint8_t key = 0;
  while (1) {
    if (kbd_ctrl_read_status() & KBD_CTRL_STATS_MASK_OUT_BUF) {
      code = kbd_enc_read_buf();
      if (code <= 0x58) {
        if (code == KEY_CAPSLOCK && is_caps == 0) { is_caps = 1; break; }
        if (code == KEY_CAPSLOCK && is_caps == 1) { is_caps = 0; break; }
        if (is_caps) {
          key = _kkybrd_scancode_std[code] - 32;
          break;
        } else {
          key = _kkybrd_scancode_std[code];
          break;
        }
      }
    }
  }
  return key;
}
