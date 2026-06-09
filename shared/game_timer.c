#include <stdint.h>

#include "game_timer.h"

static const uint64_t FALL_MS_BASE = 300;
static const uint64_t FALL_MS_DECREASE_PER_LEVEL = 30;
static const uint64_t FALL_MS_MIN = 50;

static uint64_t game_fall_interval_ms(const GameState *game) {
  uint64_t level = game_get_level(game);
  uint64_t speed_steps = level > 1 ? level - 1 : 0;
  uint64_t max_decrease = FALL_MS_BASE - FALL_MS_MIN;
  uint64_t steps_until_min = max_decrease / FALL_MS_DECREASE_PER_LEVEL;

  if (max_decrease % FALL_MS_DECREASE_PER_LEVEL != 0) {
    steps_until_min++;
  }

  if (speed_steps >= steps_until_min) {
    return FALL_MS_MIN;
  }

  return FALL_MS_BASE - (speed_steps * FALL_MS_DECREASE_PER_LEVEL);
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
