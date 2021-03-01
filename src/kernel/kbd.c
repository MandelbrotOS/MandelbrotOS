#include <hw.h>
#include <kernel/alloc.h>
#include <kernel/irq.h>
#include <kernel/kbd.h>
#include <kernel/text.h>
#include <macros.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static uint32_t kb_mode = 0;

static char kb_map[128] = {
    0,    0x1b, /* esc */
    '1',  '2',  '3',  '4',  '5', '6', '7',  '8', '9', '0',
    '-',  '=',  '\b', '\t', 'q', 'w', 'e',  'r', 't', 'y',
    'u',  'i',  'o',  'p',  '[', ']', '\n', 0, /* left ctrl */
    'a',  's',  'd',  'f',  'g', 'h', 'j',  'k', 'l', ';',
    '\'', '`',  0, /* left shift */
    '\\', 'z',  'x',  'c',  'v', 'b', 'n',  'm', ',', '.',
    '/',  0,                                             /* right shift */
    '*',  0,                                             /* alt */
    ' ',                                                 /* space*/
    0,                                                   /* capslock */
    0,    0,    0,    0,    0,   0,   0,    0,   0,   0, /* f1 ... f10 */
    0,                                                   /* num lock*/
    0,                                                   /* scroll Lock */
    0,                                                   /* home key */
    0,                                                   /* up arrow */
    0,                                                   /* page up */
    '-',  0,                                             /* left arrow */
    0,    0,                                             /* right arrow */
    '+',  0,                                             /* end key*/
    0,                                                   /* down arrow */
    0,                                                   /* page down */
    0,                                                   /* insert key */
    0,                                                   /* delete key */
    0,    0,    0,    0,                                 /* f11 key */
    0,                                                   /* f12 key */
    0, /* all other keys are undefined */
};

static char kb_shift_map[128] = {
    0,    0x1b, /* esc */
    '!',  '@',  '#',  '$',  '%', '^', '&',  '*', '(', ')',
    '_',  '+',  '\b', '\t', 'Q', 'W', 'E',  'R', 'T', 'Y',
    'U',  'I',  'O',  'P',  '{', '}', '\n', 0, /* left control */
    'A',  'S',  'D',  'F',  'G', 'H', 'J',  'K', 'L', ':',
    '\"', '~',  0, /* left shift */
    '|',  'Z',  'X',  'C',  'V', 'B', 'N',  'M', '<', '>',
    '?',  0,                                             /* right shift */
    '*',  0,                                             /* alt */
    ' ',                                                 /* space bar */
    0,                                                   /* caps lock */
    0,    0,    0,    0,    0,   0,   0,    0,   0,   0, /* f1 ... f10 */
    0,                                                   /* num lock*/
    0,                                                   /* scroll lock */
    0,                                                   /* home key */
    0,                                                   /* up arrow */
    0,                                                   /* page up */
    '-',  0,                                             /* left arrow */
    0,    0,                                             /* right arrow */
    '+',  0,                                             /* end key*/
    0,                                                   /* down arrow */
    0,                                                   /* page down */
    0,                                                   /* insert key */
    0,                                                   /* delete key */
    0,    0,    0,    0,                                 /* f11 key */
    0,                                                   /* f12 key */
    0, /* all other keys are undefined */
};

static char shift(char sc) {
  char ch = sc & 0x7f; // clear highest bit

  /* the previous scancode is 0xe0*/
  if (kb_mode & E0ESC) {
    switch (ch) {
    case 0x1D:
      return CTRL;
    case 0x38:
      return ALT;
    }
  } else {
    switch (ch) {
    case 0x2A:
    case 0x36:
      return SHIFT;
    case 0x1D:
      return CTRL;
    case 0x38:
      return ALT;
    }
  }
  return 0;
}

char mapndebounce(uint8_t scancode) {
  uint8_t sc, m;
  char ch;

  sc = scancode;

  /* is a escape char? */
  if (KB_IS_ESCAPE(sc)) {
    kb_mode |= E0ESC;
    pass;
  }

  if ((m = shift(sc))) {
    if (KB_IS_RELEASE(sc)) {
      /* clear mode when release the key */
      kb_mode &= ~m;
    } else {
      kb_mode |= m;
    }
    return 0;
  }

  /* check alt shift and ctrl */
  if (kb_mode & SHIFT) {
    ch = kb_shift_map[sc & 0x7f];
  } else {
    ch = kb_map[sc & 0x7f];
  }

  /* on release */
  if (KB_IS_RELEASE(sc)) {
    kb_mode &= ~E0ESC;
    return 0;
  }
  /* on press */
  else if (ch != 0) {
    return ch;
  }
}

void handelescape(uint8_t scancode) {
  uint8_t sc, m;
  char ch;

  sc = scancode;

  if (KB_IS_ESCAPE(sc)) {
    kb_mode |= E0ESC;
    pass;
  }

  if ((m = shift(sc))) {
    if (KB_IS_RELEASE(sc)) {
      kb_mode &= ~m;
    } else {
      kb_mode |= m;
    }
    return;
  }

  /* check alt shift and ctrl */
  if (kb_mode & SHIFT) {
    ch = kb_shift_map[sc & 0x7f];
  } else {
    ch = kb_map[sc & 0x7f];
  }

  if (kb_mode & CTRL) {
    if (ch == 'l') {
      cls();
    }
  }

  if (kb_mode & ALT) {
    /* nothing to do ? */
  }
}

void kbdhandler(register_t *r) {
  currkey = inb(KB_DATA);
  handelescape(currkey);
}

char *gets() {
  char *mallocd, *base;
  uint8_t m;
  size_t size = 0, index = 0;
  int chars_typed = 0;

  mallocd = (char *)malloc(sizeof(char));

  while (true) {
    cursor();
    char ch = mapndebounce(currkey);

    if ((inb(KB_STAT) & KB_STAT_OBF) == 0) {
      continue;
    }

    if (ch == '\n') {
      printf(" ");
      printf("\r\n");
      break;
    } else if (ch == '\b') {
      if (chars_typed != 0) {
        mallocd = (char *)realloc(mallocd, chars_typed * sizeof(char));

        chars_typed -= 1;
        printf("\b");
        mallocd[chars_typed] = ' ';
      }
    } else if (ch != '\0') {
      printf("%c", ch);

      if (chars_typed != 0) {
        mallocd = (char *)realloc(mallocd, chars_typed * sizeof(char));
      }
      mallocd[chars_typed] = ch;

      chars_typed++;
    }
  }

  mallocd[chars_typed + 1] = '\0';
  base = mallocd;
  free(mallocd);

  return base;
}

int kbd_init() {
  irq_install_handler(1, kbdhandler);
  return 0;
}