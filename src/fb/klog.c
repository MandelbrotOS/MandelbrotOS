#include <fb/fb.h>
#include <klog.h>
#include <printf.h>
#include <stdint.h>

#define KLOG_GREEN 0x11ff11
#define KLOG_RED 0xff1111
#define KLOG_YELLOW 0xffff11
#define KLOG_BLUE 0x1111ff

void klog(int type, char *message) {

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
    printf(" ] %s\r\n", message);
    break;
  case 3:
    printf("[ ");
    curr_fg_col = KLOG_BLUE;
    printf("INFO");
    curr_fg_col = old_fg;
    printf(" ] %s\r\n", message);
    break;
  }
}
