#include <font.h>
#include <kernel/init.h>
#include <kernel/kpanic.h>
#include <kernel/text.h>
#include <kernel/vbe.h>
#include <macros.h>
#include <stdbool.h>
#include <string.h>

void init_check(int func, char *name, bool ness) {
  printf("Initing %s", name);

  if (func == 0) {
    // x = fb_width - 54;
    x = fb_width - ((GLYPH_WIDTH + 1) * 6) - 2;
    printf("[ ");
    fg_color = GREEN;
    printf("OK");
    fg_color = FG;
    printf(" ]");

    inited_funcs[inited_funs_no] = name;
    inited_funs_no++;
  } else {
    x = fb_width - ((GLYPH_WIDTH + 1) * 10) - 2;
    printf("[ ");
    fg_color = RED;
    printf("FAILED");
    fg_color = FG;
    printf(" ]");
    if (ness) {
      char *failure_point;
      // sprintf(failure_point, "%s%s", "Failed init Function - ", name);
      strcpy(failure_point, "Failed init function - ");
      strcat(failure_point, name);
      kpanic(failure_point, 19);
    } else {
      pass;
    }
  }
}