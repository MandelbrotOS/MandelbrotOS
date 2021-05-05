#define HALT() { \
  __asm__ __volatile__( \
    "1:\n\t" \
    "cli\n\t" \
    "hlt\n\t" \
    "jmp 1b" \
  ); \
};
