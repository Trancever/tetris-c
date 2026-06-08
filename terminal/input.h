#ifndef INPUT_H
#define INPUT_H

#include "game.h"

typedef enum {
  INPUT_CONTINUE = 0,
  INPUT_EXIT,
  INPUT_RESET_TIMER,
} InputStatus;

void disable_raw_mode(void);

void enable_raw_mode(void);

char read_key(void);

InputStatus handle_key_press(char key, GameState *game);

#endif
