#include <fb/fb.h>
#include <klog.h>
#include <printf.h>
#include <stdarg.h>
#include <stdint.h>
#include <vprintf.h>

#define KLOG_GREEN 0x55FF55
#define KLOG_RED 0xFF5555
#define KLOG_YELLOW 0xFFFF55
#define KLOG_BLUE 0x5555FF

void klog(int type, char *message, ...) {
  va_list list;
  uint32_t old_fg = curr_fg_col;

  switch (type) {
  case 0:
    printf("[ ");
    curr_fg_col = KLOG_GREEN;
    printf("OKAY");
    curr_fg_col = old_fg;
    printf(" ] %s initialized \r\n", message);
    break;
  case 1:
    printf("[ ");
    curr_fg_col = KLOG_RED;
    printf("FAIL");
    curr_fg_col = old_fg;
    printf(" ] %s not initialized \r\n", message);
    break;
  case 2:
    printf("[ ");
    curr_fg_col = KLOG_YELLOW;
    printf("WARN");
    curr_fg_col = old_fg;
    printf(" ]");
    va_start(list, message);
    vprintf(message, list);
    break;
  case 3:
    printf("[ ");
    curr_fg_col = KLOG_BLUE;
    printf("INFO");
    curr_fg_col = old_fg;
    printf(" ] ");
    va_start(list, message);
    vprintf(message, list);
    break;
  }
  va_end(list);
}
