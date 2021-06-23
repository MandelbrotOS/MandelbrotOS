#include <boot/stivale2.h>
#include <fb/fb.h>
#include <printf.h>
#include <stddef.h>
#include <stdint.h>

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
  struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
  while (1) {
    if (!current_tag) {
      return NULL;
    } else if (current_tag->identifier == id) {
      return current_tag;
    }
    current_tag = (void *)current_tag->next;
  }
}

void kernel_main(struct stivale2_struct *bootloader_info) {
  struct stivale2_struct_tag_framebuffer *framebuffer_info =
      (struct stivale2_struct_tag_framebuffer *)stivale2_get_tag(
          bootloader_info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

  init_fb(framebuffer_info);

  printf("Hello, world!\r\n");

  while (1)
    ;
}
