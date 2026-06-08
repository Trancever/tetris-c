#ifndef BOARD_H
#define BOARD_H

#include "cell.h"
#include "piece.h"

#define BOARD_WIDTH 12
#define BOARD_VISIBLE_HEIGHT 20
#define BOARD_HIDDEN_HEIGHT 4
#define BOARD_HEIGHT (BOARD_VISIBLE_HEIGHT + BOARD_HIDDEN_HEIGHT)

typedef struct {
  Cell cells[BOARD_HEIGHT * BOARD_WIDTH];
} Board;

const Cell *get_board_cell_const(const Board *board, int x, int y);

bool board_lock_piece(Board *board, const Piece *piece, int piece_x,
                      int piece_y);

void board_overlay_piece_for_render(Board *board, const Piece *piece,
                                    int piece_x, int piece_y);

bool board_can_place_piece(const Board *board, const Piece *piece, int piece_x,
                           int piece_y);

bool board_can_move_piece_down(const Board *board, const Piece *piece,
                               int piece_x, int piece_y);

bool board_can_move_piece_left(const Board *board, const Piece *piece,
                               int piece_x, int piece_y);

bool board_can_move_piece_right(const Board *board, const Piece *piece,
                                int piece_x, int piece_y);

int board_clear_completed_rows(Board *board);

#endif
