#include <stdint.h>

#include "game_timer.h"

static const uint64_t FALL_MS_BASE = 300;
static const uint64_t FALL_MS_DECREASE_PER_LEVEL = 30;
static const uint64_t FALL_MS_MIN = 50;

static uint64_t game_fall_interval_ms(const GameState *game) {
  uint64_t level = game_get_level(game);
  uint64_t decrease = (level - 1) * FALL_MS_DECREASE_PER_LEVEL;
  uint64_t max_decrease = FALL_MS_BASE - FALL_MS_MIN;

  if (decrease >= max_decrease) {
    return FALL_MS_MIN;
  }

  return FALL_MS_BASE - decrease;
}

void game_timer_reset(GameTimer *timer) { timer->elapsed_ms = 0; }

bool game_timer_update(GameTimer *timer, const GameState *game,
                       uint64_t elapsed_ms) {
  uint64_t fall_interval_ms = game_fall_interval_ms(game);

  timer->elapsed_ms += elapsed_ms;

  if (timer->elapsed_ms < fall_interval_ms) {
    return false;
  }

  timer->elapsed_ms -= fall_interval_ms;
  return true;
}
