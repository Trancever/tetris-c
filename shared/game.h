#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#include "board.h"
#include "piece.h"

typedef enum {
  GAME_RUNNING,
  GAME_PAUSED,
  GAME_OVER
} GameStatus;

typedef struct {
  Board board;
  Piece current_piece;
  Piece next_piece;
  int piece_x;
  int piece_y;
  uint64_t score;
  GameStatus game_status;
} GameState;

GameState initialize_game(void);

void move_piece_left(GameState *game);

void move_piece_right(GameState *game);

void rotate_piece(GameState *game);

void game_step_down(GameState *game);

uint64_t game_get_level(const GameState *game);

void game_toggle_pause(GameState *game);

void game_restart(GameState *game);

#endif
