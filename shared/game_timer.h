#ifndef GAME_TIMER_H
#define GAME_TIMER_H

#include <stdbool.h>
#include <stdint.h>

#include "game.h"

typedef struct {
  uint64_t elapsed_ms;
} GameTimer;

void game_timer_reset(GameTimer *timer);

bool game_timer_update(GameTimer *timer, const GameState *game,
                       uint64_t elapsed_ms);

#endif
