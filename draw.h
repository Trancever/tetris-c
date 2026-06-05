#ifndef DRAW_H
#define DRAW_H

#include "board.h"

#define BOARD_CELL_WIDTH 2
#define BOARD_BORDER_VERTICAL "|"
#define BOARD_BORDER_HORIZONTAL "-"
#define BOARD_CELL_FILLED "■"
#define BOARD_CELL_EMPTY " "

int draw_board(int top, int left, const Board *board);

int draw_score(int top, int left, int score);

int draw_level(int top, int left, int level);

int draw_legend(int top, int left);

int draw_next_piece(int top, int left, const Piece *piece);

void terminal_clear(void);

#endif
