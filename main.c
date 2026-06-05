#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "draw.h"
#include "engine.h"
#include "game.h"
#include "input.h"

#define FRAME_MS 33
#define FALL_MS_BASE 300
#define FALL_MS_DECREASE_PER_LEVEL 30

int main(void) {
  srand((unsigned int)time(NULL));
  enable_raw_mode();

  terminal_clear();

  GameState game = initialize_game();

  int fall_timer = 0;

  while (true) {
    int fall_ms = FALL_MS_BASE -
                  ((game_get_level(&game) - 1) * FALL_MS_DECREASE_PER_LEVEL);
    if (fall_ms < 50) {
      fall_ms = 50;
    }

    char key = read_key();

    if (key == 'q') {
      exit(0);
    } else if (key == 't') {
      game_toggle_pause(&game);
      fall_timer = 0;
    } else if (key == 'r') {
      game_restart(&game);
      fall_timer = 0;
    }

    if (game.game_status == GAME_RUNNING) {
      fall_timer += FRAME_MS;

      if (key == 'a') {
        move_piece_left(&game);
      } else if (key == 'd') {
        move_piece_right(&game);
      } else if (key == 'w') {
        rotate_piece(&game);
      } else if (key == 's') {
        game_step_down(&game);
      }

      if (fall_timer >= fall_ms) {
        game_step_down(&game);

        fall_timer = 0;
      }
    }

    draw_game(&game);
    fflush(stdout);
    sleep_ms(FRAME_MS);
  }
}
