#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "draw.h"
#include "engine.h"
#include "game.h"
#include "game_timer.h"
#include "input.h"

#define FRAME_MS 33

int main(void) {
  srand((unsigned int)time(NULL));
  enable_raw_mode();

  terminal_clear();

  GameState game = initialize_game();
  GameTimer game_timer = {0};
  uint64_t last_time = current_time_ms();

  while (true) {
    uint64_t current_time = current_time_ms();
    uint64_t elapsed_ms = current_time - last_time;
    last_time = current_time;

    char key = read_key();
    InputStatus input_status = handle_key_press(key, &game);

    if (input_status == INPUT_EXIT) {
      exit(0);
    }

    if (input_status == INPUT_RESET_TIMER) {
      game_timer_reset(&game_timer);
    }

    if (game.game_status == GAME_RUNNING &&
        game_timer_update(&game_timer, &game, elapsed_ms)) {
      game_step_down(&game);
    }

    draw_game(&game);
    fflush(stdout);
    sleep_ms(FRAME_MS);
  }
}
