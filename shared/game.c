#include <stdint.h>
#include <stdio.h>

#include "board.h"
#include "game.h"
#include "piece.h"

static void game_set_initial_piece_position(GameState *game) {
  game->piece_x = (BOARD_WIDTH - game->current_piece.width) / 2;
  // Always place the piece at the bottom most hidden row (one row above the visible board rows)
  game->piece_y = BOARD_HIDDEN_HEIGHT - 1;
}

GameState initialize_game(void) {
  GameState game = {0};

  game.game_status = GAME_RUNNING;

  game.next_piece = create_random_piece();
  game.current_piece = create_random_piece();
  game_set_initial_piece_position(&game);

  return game;
}

static void generate_new_piece(GameState *game) {
  game->current_piece = game->next_piece;
  
  game_set_initial_piece_position(game);
  
  if (!board_can_place_piece(&game->board, &game->current_piece, game->piece_x, game->piece_y)) {
    game->game_status = GAME_OVER;
    return;
  }
  
  game->next_piece = create_random_piece();
}

static void move_piece_down(GameState *game) { game->piece_y++; }

void move_piece_left(GameState *game) {
  bool can_move = board_can_move_piece_left(&game->board, &game->current_piece,
                                            game->piece_x, game->piece_y);

  if (can_move) {
    game->piece_x--;
  }
}

void move_piece_right(GameState *game) {
  bool can_move = board_can_move_piece_right(&game->board, &game->current_piece,
                                             game->piece_x, game->piece_y);

  if (can_move) {
    game->piece_x++;
  }
}

static int get_rotated_piece_x(const Piece *old_piece,
                               const Piece *rotated_piece, int piece_x) {
  int delta_width = old_piece->width - rotated_piece->width;
  int new_x = piece_x + delta_width / 2;

  if (new_x < 0) {
    new_x = 0;
  } else if (new_x + rotated_piece->width > BOARD_WIDTH) {
    new_x = BOARD_WIDTH - rotated_piece->width;
  }

  return new_x;
}

void rotate_piece(GameState *game) {
  Piece rotated_piece = create_rotated_piece(&game->current_piece);
  int new_x =
      get_rotated_piece_x(&game->current_piece, &rotated_piece, game->piece_x);

  if (!board_can_place_piece(&game->board, &rotated_piece, new_x,
                             game->piece_y)) {
    return;
  }

  game->current_piece = rotated_piece;
  game->piece_x = new_x;
}

void game_step_down(GameState *game) {
  bool can_move_down = board_can_move_piece_down(
      &game->board, &game->current_piece, game->piece_x, game->piece_y);

  if (can_move_down) {
    move_piece_down(game);
  } else {
    bool touched_hidden_rows = board_lock_piece(
        &game->board, &game->current_piece, game->piece_x, game->piece_y);

    if (touched_hidden_rows) {
      game->game_status = GAME_OVER;
      return;
    }

    int cleared_rows = board_clear_completed_rows(&game->board);
    game->score += (uint64_t)cleared_rows * (uint64_t)cleared_rows;

    generate_new_piece(game);
  }
}

uint64_t game_get_level(const GameState *game) {
  if (game->score == 0) {
    return 1;
  }

  return game->score / 10 + 1;
}

void game_toggle_pause(GameState *game) {
  if (game->game_status == GAME_OVER) {
    return;
  }

  game->game_status = game->game_status == GAME_RUNNING ? GAME_PAUSED : GAME_RUNNING;
}

void game_restart(GameState *game) {
  *game = initialize_game();
}
