#include <kernel/cpuid.h>
#include <kernel/kbd.h>
#include <kernel/kpanic.h>
#include <kernel/kshell.h>
#include <kernel/pit.h>
#include <kernel/power.h>
#include <kernel/text.h>
#include <kernel/vbe.h>
#include <kernelinf.h>
#include <macros.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

void prompt() { printf("PROMPT> "); }

void sysfetch() {
  printf("%s\r\n", KERNEL_ARTS);

  // Value chosen to be 1 hour, 1 minute, 1 second, and 1 millisecond
  long milli = timer_ticks;
  // 3600000 milliseconds in an hour
  long hr = milli / 3600000;
  milli = milli - 3600000 * hr;
  // 60000 milliseconds in a minute
  long min = milli / 60000;
  milli = milli - 60000 * min;

  // 1000 milliseconds in a second
  long sec = milli / 1000;
  milli = milli - 1000 * sec;

  printf("\r\nKernel name: %s\r\nKernel version: %s\r\nKernel build date: %s "
         "%s\r\nUptime: %u hours %u minutes %u seconds\r\nCPU model number: "
         "%u\r\n",
         KERNEL_NAME, KERNEL_VERS, KENREL_DATE, KERNEL_TIME, hr, min, sec,
         get_model());
  bg_color = RED;
  printf(" ");
  bg_color = GREEN;
  printf(" ");
  bg_color = YELLOW;
  printf(" ");
  bg_color = BLUE;
  printf(" ");
  bg_color = MAGENTA;
  printf(" ");
  bg_color = CYAN;
  printf(" ");
  bg_color = WHITE;
  printf(" ");
  bg_color = GRAY;
  printf(" ");
  bg_color = FG;
  printf(" ");

  printf("\r\n");

  bg_color = DARK_RED;
  printf(" ");
  bg_color = DARK_GREEN;
  printf(" ");
  bg_color = DARK_YELLOW;
  printf(" ");
  bg_color = DARK_BLUE;
  printf(" ");
  bg_color = DARK_MAGENTA;
  printf(" ");
  bg_color = DARK_CYAN;
  printf(" ");
  bg_color = BLACK;
  printf(" ");
  bg_color = DARK_GRAY;
  printf(" ");
  bg_color = BG;
  printf(" ");

  bg_color = BG;
  printf("\r\n\n");
}

int kshell(multiboot_info_t *mbi, unsigned long magic) {
  while (true) {
    prompt();

    char *args = gets();
    char line[strlen(args) + 1];
    const char *argv[wspaceamount(args) + 1];
    int argc = 0;

    for (int i = 0; i < strlen(args) + 1; i++) {
      line[i] = args[i];
    }

    argv[argc] = strtok(line, " ");
    while (argv[argc]) {
      argc++;
      argv[argc] = strtok(0, " ");
    }

    if (check_cmd("hello")) {
      printf("Hello, world!\r\n");
    } else if (check_cmd("mandelbrot")) {
      if (argc == 5) {
        // int inval = false;
        // for (int i = 1; i != 4; i++)
        // {
        //    if (isdigit(argv[i]) == 0)
        //    {
        //       inval = true;
        //    }
        // }
        // if (inval)
        // {
        //    printf("Invalid input! Requires float!\r\n");
        // }
        // else
        // {
        mandelbrot(atof(argv[1]), atof(argv[2]), atof(argv[3]), atof(argv[4]),
                   0x000000);
        //    }
      } else if (argc > 1 && argc < 5 || argc > 5) {
        printf("%s: Invalid amount of args!\r\n", argv[0]);
      } else if (argc == 1) {
        mandelbrot(-1.95, -1.2, 2.5, 2.5, 0x000000);
      }
    } else if (check_cmd("bsod")) {
      printf("Warning this function will BSOD (Blue Screen of Death) your "
             "system. This is usually bad.\r\nContinue y/[N]: ");
      char *confirm = gets();
      for (int i = 0; confirm[i]; i++) {
        confirm[i] = tolower(confirm[i]);
      }

      if (strcmp(confirm, "yes") || strcmp(confirm, "y")) {
        kpanic("User induced BSOD", 20);
      } else {
        printf("Aborted BSOD!\r\n");
      }
    } else if (check_cmd("clsg")) {
      drawrect(0, 0, fb_width, fb_height, GREEN);
      x = border;
      y = border;
    } else if (check_cmd("cls")) {
      cls();
    } else if (check_cmd("hi")) {
      for (int p = 0; p != 20; p++) {
        printf("hi\r\n");
      }
    } else if (check_cmd("cpuid")) {
      get_vendor();
    } else if (check_cmd("sysfetch")) {
      sysfetch();
    } else if (check_cmd("reboot")) {
      reboot();
    } else if (check_cmd(0)) {
      pass;
    } else {
      fg_color = RED;
      printf("%s", argv[0]);
      fg_color = FG;
      printf(": Not a valid command!\r\n");
    }
  }

  return 0;
}
