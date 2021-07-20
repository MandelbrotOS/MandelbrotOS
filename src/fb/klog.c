#include <fb/fb.h>
#include <klog.h>
#include <lock.h>
#include <printf.h>
#include <stdarg.h>
#include <stdint.h>
#include <vprintf.h>

#define KLOG_GREEN 0x55FF55
#define KLOG_RED 0xFF5555
#define KLOG_YELLOW 0xFFFF55
#define KLOG_BLUE 0x5555FF

void klog(int type, char *message, ...) {
  MAKE_LOCK(klog_lock);

  va_list list;
  uint32_t old_fg = curr_fg_col;

  switch (type) {
  case 0:;
    printf("[ ");
    set_fg(KLOG_GREEN);
    printf("OKAY");
    set_fg(old_fg);
    printf(" ] %s initialized \r\n", message);
    break;
  case 1:;
    printf("[ ");
    set_fg(KLOG_RED);
    printf("FAIL");
    set_fg(old_fg);
    printf(" ] %s not initialized \r\n", message);
    break;
  case 2:;
    printf("[ ");
    set_fg(KLOG_YELLOW);
    printf("WARN");
    set_fg(old_fg);
    printf(" ] ");
    va_start(list, message);
    vprintf(message, list);
    break;
  case 3:;
    printf("[ ");
    set_fg(KLOG_BLUE);
    printf("INFO");
    set_fg(old_fg);
    printf(" ] ");
    va_start(list, message);
    vprintf(message, list);
    break;
  }
  va_end(list);

  UNLOCK(klog_lock);
}
