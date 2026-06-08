#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

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

InputStatus handle_key_press(char key, GameState *game) {
  if (key == 'q') {
    return INPUT_EXIT;
  } else if (key == 't') {
    game_toggle_pause(game);
    return INPUT_RESET_TIMER;
  } else if (key == 'r') {
    game_restart(game);
    return INPUT_RESET_TIMER;
  }

  if (game->game_status == GAME_RUNNING) {
    if (key == 'a') {
      move_piece_left(game);
    } else if (key == 'd') {
      move_piece_right(game);
    } else if (key == 'w') {
      rotate_piece(game);
    } else if (key == 's') {
      game_step_down(game);
    }
  }

  return INPUT_CONTINUE;
}
