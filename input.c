#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

#include "input.h"

static struct termios original_termios;

void disable_raw_mode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &original_termios);
    atexit(disable_raw_mode);

    struct termios raw = original_termios;

    const tcflag_t raw_mode_flags = (tcflag_t)ECHO | (tcflag_t)ICANON;
    raw.c_lflag &= ~raw_mode_flags;
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

char read_key(void) {
  char c;

  if (read(STDIN_FILENO, &c, 1) == 1) {
      return c;
  }

  return '\0';
}
