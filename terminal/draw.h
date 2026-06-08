#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>

#include "board.h"
#include "game.h"
#include "piece.h"

int draw_board(int top, int left, const Board *board);

int draw_score(int top, int left, uint64_t score);

int draw_level(int top, int left, uint64_t level);

int draw_legend(int top, int left);

int draw_next_piece(int top, int left, const Piece *piece);

void terminal_clear(void);

void draw_game(const GameState *game);

#endif
