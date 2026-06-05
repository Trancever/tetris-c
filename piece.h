#ifndef PIECE_H
#define PIECE_H

#include "cell.h"

#define PIECE_CELLS_TOTAL_WIDTH 4
#define PIECE_CELLS_TOTAL_HEIGHT 4

typedef struct {
  int width;
  int height;
  Cell cells[PIECE_CELLS_TOTAL_WIDTH * PIECE_CELLS_TOTAL_HEIGHT];
} Piece;

typedef enum {
  PIECE_O,
  PIECE_I,
  PIECE_S,
  PIECE_Z,
  PIECE_L,
  PIECE_J,
  PIECE_T,
  PIECE_COUNT
} PieceType;

const Cell *get_piece_cell_const(const Piece *piece, int x, int y);

Piece create_random_piece(void);

Piece create_rotated_piece(const Piece *piece);

#endif
